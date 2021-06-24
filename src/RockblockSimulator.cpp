#include "RockblockSimulator.hpp"

RockblockSimulator::RockblockSimulator() {
    baud = 0;
    mo_status = 0;
    momsn = 0;
    mt_status = 0;
    mtmsn = 0;
    mt_len = 0;
    mt_queue_len = 0;
    flush_stage = 0;
    bin_transmit = 0;
}

void RockblockSimulator::begin(uint32_t baud) {
    this->baud = baud;
}

int RockblockSimulator::available() {
    serial_check();
    serial_process();
    return output.size();
}

size_t RockblockSimulator::write(uint8_t c) {
    input += (char) c;
    serial_check();
    serial_process();
    return 1;
}

size_t RockblockSimulator::print(const char* s) {
    input += s;
    serial_check();
    serial_process();
    return strlen(s);
}

int RockblockSimulator::read() {
    char c = 0;
    if( available() ) {
        c = output[output.size() - 1];
        output = output.substr(0, output.size() - 1);
    }
    serial_check();
    serial_process();
    return (int) c;
}

bool RockblockSimulator::insert(std::string s) {
    if(s.length() == 20) {
        std::string tmp = "";
        for(size_t i = 0; i < 20; i += 2) {
            const char* sub = s.substr(i, 2).c_str();
            char c = strtol(sub, nullptr, 16);
            tmp += c;
        }
        mt_queue.push_back(tmp);
        return true;
    } else {
        return false;
    }
}

bool RockblockSimulator::insert(const char* s) {
    std::string str = s;
    return insert(str);
}

void RockblockSimulator::serial_check() {
    while(Serial.available()) {
        char c = Serial.read();
        if( !(c == '\r' || c == '\n') ) {
            interface += c;
        }
        if(c == '\n') {
            Serial.println();
            if( insert(interface) ) {
                Serial.print("SIM INSERT: ");
                Serial.println(interface.c_str());
            }
            interface.clear();
        }
    }
}

void RockblockSimulator::flush_check() {
    if(flush_stage == 1) {
        flush_stage = 2;
    } else if(flush_stage == 2) {
        while(mt_queue.size()) {
            mt_queue.pop_back();
        }
        mt_queue.push_back("FLUSH_MT");
        flush_stage = 0;
    }
}

void RockblockSimulator::serial_process() {
    if( input.back() == '\r' ) { // process on line end
        // print received data
        std::string tmp = input;
        std::replace( tmp.begin(), tmp.end(), '\r', 'r');
        std::replace( tmp.begin(), tmp.end(), '\n', 'n');
        Serial.print("SIM RECEIVE: ");
        if(tmp.length() > 50) {
            Serial.println("<data>");
        } else {
            Serial.println(tmp.c_str());
        }

        if( input == "AT\r" ) { // check AT
            output = "AT\r\r\nOK\r\n"; // reply OK
        } else if( input == "AT+CSQ\r" ) { // signal quality check
            output = "AT+CSQ\r\r\n+CSQ:5\r\n\r\nOK\r\n"; // replay with signal 5
        } else if( input == "AT&K0\r" ) { // flow control
            output = "AT&K0\r\r\nOK\r\n"; // reply OK
        } else if( input.find("AT+SBDWT=") == 0 ) { // transmit ASCII
            size_t idx1 = input.find('=') + 1;
            size_t idx2 = input.find('\r');
            downlink_data = input.substr(idx1, idx2 - idx1);
            if(downlink_data == "FLUSH_MT") {
                flush_stage = 1;
            }
            output = input;
            output += "\r\nOK\r\n"; // reply OK
        } else if( input.find("AT+SBDWB=") == 0 ) { // transmit binary (length)
            size_t idx1 = input.find('=') + 1;
            size_t idx2 = input.find('\r');
            std::string tmp = input.substr(idx1, idx2 - idx1);
            const char* ptr = tmp.c_str();
            send_len = strtol(ptr, nullptr, 10); // length from ascii to dec
            bin_transmit = 1; // prepare for binary transmission
            output = input;
            output += "\r\nREADY\r\n"; // reply READY
        // } else if( send_len != 0 && (int) input.size() == send_len + 2 + 1 ) {
        } else if(bin_transmit) { // transmit binary (data)
            // checksum calculation
            uint16_t checksum = 0;
            for(size_t i = 0; i < send_len; i++) {
                checksum += (uint16_t) input[i];
            }
            char b1 = checksum >> 8;
            char b2 = checksum & 0xFF;
            // revert stage flag
            bin_transmit = 0;
            // confirm checksum
            if(b1 == input[send_len] && b2 == input[send_len + 1]) {
                output = "\r\n0\r\n\r\nOK\r\n"; // reply CORRECT
                downlink_data = input.substr(0, send_len);
            } else {
                output = "\r\n2\r\n\r\nOK\r\n"; // reply ERROR
            }
        } else if( input == "AT+SBDIX\r" ) {
            flush_check();
            // MO_STATUS: successful transmission (TBD)
            mo_status = 0;
            // MOMSN: cycle num, not necessary
            momsn++;
            // MT_STATUS: message waiting or no messages
            if(mt_queue.size()) {
                mt_status = 1;
                uplink_data = mt_queue[0];
                mt_queue.pop_front();
            } else {
                mt_status = 0;
            }
            // MTMSN: cycle num, not necessary
            mtmsn++;
            // MT_LEN: message length
            mt_len = uplink_data.length();
            // MT_QUEUE_LEN: num messages waiting
            mt_queue_len = mt_queue.size();
            // print downlinked data
            Serial.print("DOWNLINK: ");
            if(downlink_data.length() == 0) {
                Serial.println("-");
            } else {
                Serial.println("<data>");
                // Serial.println(downlink_data.c_str());
                downlink_data.clear();
            }
            // Response
            output = "AT+SBDIX\r\r\n+SBDIX: ";
            // using stringstream to append ints
            std::stringstream ss;
            ss << (int) mo_status;
            ss << ", ";
            ss << (int) momsn;
            ss << ", ";
            ss << (int) mt_status;
            ss << ", ";
            ss << (int) mtmsn;
            ss << ", ";
            ss << (int) mt_len;
            ss << ", ";
            ss << (int) mt_queue_len;
            output += ss.str();
            output += "\r\n\r\nOK\r\n"; // reply OK
        } else if( input == "AT+SBDRB\r" ) { // offload binary data
            output = "AT+SBDRB\r";
            output += (char) 0;
            output += (char) mt_len; // (assumes data less than 127)
            output += uplink_data;
            uint16_t checksum = std::accumulate(uplink_data.begin(), uplink_data.end(), 0); // calculate checksum
            output += (char) (checksum >> 8);
            output += (char) (checksum & 0xFF);
            output += "\r\nOK\r\n"; // reply OK
            uplink_data.clear();
        }

        tmp = output;
        std::replace( tmp.begin(), tmp.end(), '\r', 'r');
        std::replace( tmp.begin(), tmp.end(), '\n', 'n');
        Serial.print("SIM REPLY: ");
        Serial.println(tmp.c_str());

        std::reverse( output.begin(), output.end() ); // reverse string for proper extraction
        input.clear();
    }
}
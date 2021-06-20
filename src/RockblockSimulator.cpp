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
}

void RockblockSimulator::execute() {
    while(Serial.available()) {
        char c = Serial.read();
        if( !(c == '\r' || c == '\n') ) {
            interface += c;
        }
        if(c == '\n') {
            Serial.println();
            if( interface.length() == 20 ) {
                Serial.print("ADDED: ");
                Serial.println(interface.c_str());

                std::string tmp = "";
                for(size_t i = 0; i < 20; i += 2) {
                    const char* sub = interface.substr(i, 2).c_str();
                    char c = strtol(sub, nullptr, 16);
                    tmp += c;
                }

                mt_queue.push_back(tmp);
            }
            interface.clear();
        }
    }
}

void RockblockSimulator::begin(uint32_t baud) {
    this->baud = baud;
}

int RockblockSimulator::available() {
    execute();
    process();
    return output.size();
}

size_t RockblockSimulator::write(uint8_t c) {
    input += (char) c;
    execute();
    process();
    return 1;
}

size_t RockblockSimulator::print(const char* s) {
    input += s;
    execute();
    process();
    return strlen(s);
}

int RockblockSimulator::read() {
    char c = 0;
    if( available() ) {
        c = output[output.size() - 1];
        output = output.substr(0, output.size() - 1);
    }
    execute();
    process();
    return (int) c;
}

void RockblockSimulator::check_flush() {
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

void RockblockSimulator::process() {
    if( input.back() == '\r' ) {
        std::string tmp = input;
        std::replace( tmp.begin(), tmp.end(), '\r', 'r');
        std::replace( tmp.begin(), tmp.end(), '\n', 'n');
        Serial.print("RECEIVED: ");
        if(tmp.length() > 50) {
            Serial.println("<data>");
        } else {
            Serial.println(tmp.c_str());
        }

        if( input == "AT\r" ) {
            output = "AT\r\r\nOK\r\n";
        } else if( input == "AT+CSQ\r" ) {
            output = "AT+CSQ\r\r\n+CSQ:5\r\n\r\nOK\r\n";
        } else if( input == "AT&K0\r" ) {
            output = "AT&K0\r\r\nOK\r\n";
        } else if( input.find("AT+SBDWB=") == 0 ) {
            size_t idx1 = input.find('=') + 1;
            size_t idx2 = input.find('\r');
            std::string tmp = input.substr(idx1, idx2 - idx1);
            const char* ptr = tmp.c_str();
            send_len = strtol(ptr, nullptr, 10);
            output = input;
            output += "\r\nREADY\r\n";
        } else if( send_len != 0 && (int) input.size() == send_len + 2 + 1 ) {
            uint16_t checksum = 0;
            for(size_t i = 0; i < send_len; i++) {
                checksum += (uint16_t) input[i];
            }
            char b1 = checksum >> 8;
            char b2 = checksum & 0xFF;

            if(b1 == input[send_len] && b2 == input[send_len + 1]) {
                output = "\r\n0\r\n\r\nOK\r\n";
                downlink_data = input.substr(0, send_len);
            } else {
                output = "\r\n2\r\n\r\nOK\r\n";
            }
        } else if( input == "AT+SBDIX\r" ) {
            check_flush();
            // MO_STATUS
            mo_status = 0;
            // MOMSN
            momsn++;
            // MT_STATUS
            if(mt_queue.size()) {
                mt_status = 1;
                uplink_data = mt_queue[0];
                mt_queue.pop_front();
            } else {
                mt_status = 0;
            }
            // MTMSN
            mtmsn++;
            // MT_LEN
            mt_len = uplink_data.length();
            // MT_QUEUE_LEN
            mt_queue_len = mt_queue.size();
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
            // OK
            output += "\r\n\r\nOK\r\n";
        } else if( input == "AT+SBDRB\r" ) {
            // Response Echo
            output = "AT+SBDRB\r";
            // Message Length
            output += (char) 0;
            output += (char) mt_len;
            // Binary Message
            output += uplink_data;
            // Checksum
            uint16_t checksum = std::accumulate(uplink_data.begin(), uplink_data.end(), 0);
            output += (char) (checksum >> 8);
            output += (char) (checksum & 0xFF);
            // OK
            output += "\r\nOK\r\n";
            uplink_data.clear();
        } else if( input == "AT+SBDWT=FLUSH_MT\r" ) {
            flush_stage = 1;
            output = "AT+SBDWT=FLUSH_MT\r\r\nOK\r\n";
        }

        tmp = output;
        std::replace( tmp.begin(), tmp.end(), '\r', 'r');
        std::replace( tmp.begin(), tmp.end(), '\n', 'n');
        Serial.print("SENT: ");
        Serial.println(tmp.c_str());

        std::reverse( output.begin(), output.end() ); // reverse string for proper extraction
        input.clear();
    }
}
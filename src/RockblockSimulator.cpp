#include "RockblockSimulator.hpp"

RockblockSimulator::RockblockSimulator() {
    baud = 0;
}

void RockblockSimulator::begin(uint32_t baud) {
    this->baud = baud;
}

int RockblockSimulator::available() {
    return output.size();
}

size_t RockblockSimulator::write(uint8_t c) {
    input += (char) c;
    process();
    return 1;
}

size_t RockblockSimulator::print(const char* s) {
    input += s;
    process();
    return strlen(s);
}

int RockblockSimulator::read() {
    char c = 0;
    if( available() ) {
        c = output[output.size() - 1];
        output = output.substr(0, output.size() - 1);
    }
    return (int) c;
}


void RockblockSimulator::process() {
    if( input.back() == '\r' ) {
        if( input == "AT\r" ) {
            // OK reply
            output = "AT\r\r\nOK\r\n";
        } else if( input == "AT+CSQ\r" ) {
            // simulating signal strength of 5
            // OK reply
            output = "AT+CSQ\r\r\n+CSQ:5\r\n\r\nOK\r\n";
        } else if( input == "AT&K0\r" ) {
            // OK reply
            output = "AT&K0\r\r\nOK\r\n";
        } else if( input == "AT+SBDWB=70\r" ) {
            // READY reply
            output = "AT+SBDWB=70\r\r\nREADY\r\n";
        } else if( input.size() == 73 ) {
            // checksum calculation
            // OK reply
            uint16_t checksum = 0;
            for(size_t i = 0; i < 70; i++) {
                checksum += (uint16_t) input[i];
            }
            char b1 = checksum >> 8;
            char b2 = checksum & 0xFF;

            if(b1 == input[70] && b2 == input[71]) {
                output = "\r\n0\r\n\r\nOK\r\n";
            } else {
                output = "\r\n2\r\n\r\nOK\r\n";
            }
        } else if( input == "AT+SBDIX\r" ) {
            // Message downlinked & 10 byte message ready for uplink
            output = "AT+SBDIX\r\r\n+SBDIX: 0, 5, 1, 2, 10, 0\r\n\r\nOK\r\n";
        } else if( input == "AT+SBDRB\r" ) {
            // TODO
            output = "AT+SBDRB\r";
            // Message Length
            output += (char) 0;
            output += (char) 10;
            // Binary Message
            output += (char) 10;
            output += (char) 11;
            output += (char) 12;
            output += (char) 13;
            output += (char) 14;
            output += (char) 15;
            output += (char) 1;
            output += (char) 2;
            output += (char) 3;
            output += (char) 4;
            // Checksum
            output += (char) 0;
            output += (char) 85;
            output += "\r\nOK\r\n";
        }
        std::reverse( output.begin(), output.end() ); // reverse string for proper extraction
        input.clear();
    }
}
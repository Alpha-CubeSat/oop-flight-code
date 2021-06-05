#include "RockblockSimulator.hpp"

RockblockSimulator::RockblockSimulator() {
    baud = 0;
}

void RockblockSimulator::begin(uint32_t baud) {
    this->baud = baud;
}

int RockblockSimulator::available() {
    return input.str().size();
}

size_t RockblockSimulator::write(uint8_t c) {
    input << (char) c;
    process();
    return 1;
}

size_t RockblockSimulator::print(const char* s) {
    input << s;
    process();
    return strlen(s);
}

int RockblockSimulator::read() {
    int c = 0;
    if( available() ) {
        output >> c;
    }
    return c;
}

void RockblockSimulator::process() {
    if( input.peek() == '\r' ) {
        input >> buffer;
        if( buffer == "AT\r" ) {
            output << "AT\r\r\nOK\r\n";
        } else if( buffer == "AT+CSQ\r" ) {
            // TODO: select signal strength 0->5
            output << "AT+CSQ\r\r\n+CSQ:0\r\n\r\nOK\r\n";
        } else if( buffer == "AT&K\r" ) {
            output << "AT&K0\r\r\nOK\r\n";
        } else if( buffer == "AT+SBDWB=70\r" ) {
            output << "AT+SBDWB=70\r\r\nREADY\r\n";
        } else if( buffer.size() == 73 ) {
            // TODO: verify checksum?
            output << "\r\n0\r\n\r\nOK\r\n";
        } else if( buffer == "AT+SBDIX\r" ) {
            // TODO: process MO / MT
            // requires active rockblock for mailbox check
            // https://docs.rockblock.rock7.com/docs/receive-data
            output << "AT+SBDIX\r";
        }
        buffer.clear();
    }
}
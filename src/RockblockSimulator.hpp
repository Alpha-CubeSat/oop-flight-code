#ifndef ROCKBLOCK_SIMULATOR_HPP_
#define ROCKBLOCK_SIMULATOR_HPP_

#include <sstream>
#include <string>
#include <algorithm>
#include <deque>
#include <numeric>
#include "Arduino.h"

class RockblockSimulator {
    public:
        RockblockSimulator();
        void begin(uint32_t baud);
        int available();
        size_t write(uint8_t c);
        size_t print(const char* s);
        int read();
    private:
        void serial_check();
        void flush_check();
        void serial_process();
        uint32_t baud;
        std::string input;
        std::string output;
        std::string interface;
        std::deque<std::string> mt_queue;
        std::string uplink_data;
        std::string downlink_data;
        uint16_t send_len;
        uint8_t mo_status;
        uint16_t momsn;
        uint8_t  mt_status;
        uint16_t mtmsn;
        uint16_t mt_len;
        uint8_t mt_queue_len;
        uint8_t flush_stage;
        uint8_t bin_transmit;
};

#endif
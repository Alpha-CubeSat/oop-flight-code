#ifndef ROCKBLOCK_SIMULATOR_HPP_
#define ROCKBLOCK_SIMULATOR_HPP_

#include "sfr.hpp"

class RockblockSimulator {
    public:
        RockblockSimulator();
        void begin(uint32_t baud);
        int available();
        size_t write(uint8_t c);
        size_t print(const char* s);
        int read();
    private:
        void process();
        uint32_t baud;
        std::stringstream input;
        std::stringstream output;
        std::string buffer;
};

#endif
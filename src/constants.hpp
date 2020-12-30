#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <HardwareSerial.h>

namespace constants{
    namespace gps{
        constexpr int baud = 9600;
        constexpr int mand_deploy = 28000;
    }
    namespace photoresistor{
        constexpr int pin = 22;
        constexpr int light_val = 4;
    }
    namespace burnwire{
        constexpr int pin = 35;
    }
    namespace rockblock{
        constexpr int baud = 19200;
        constexpr uint16_t receive_buffer_size = 240;
        constexpr uint16_t send_buffer_size = 300;
        constexpr int command_size = 6;
        constexpr int opcode_deploy = 1;
    }
};

#endif

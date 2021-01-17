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
        constexpr unsigned long two_hours = 7200000;
        constexpr unsigned long ten_minutes = 600000;
    }
    namespace temperature{
        constexpr int pin = 16;
    }
    namespace current{
        constexpr int pin = 15;
    }
    namespace acs{
        constexpr int yout1 = 5; 
        constexpr int yout2 = 6; 
        constexpr int zout1 = 9; 
        constexpr int zout2 = 10; 
        constexpr int xout1 = 7; 
        constexpr int xout2 = 8; 
        constexpr int xPWMpin = 2; 
        constexpr int yPWMpin = 3; 
        constexpr int zPWMpin = 4; 
        constexpr int STBYpin = 21; 
        constexpr int STBZpin = 20; 
        constexpr int xtorqorder = 1;
        constexpr int ytorqorder = 1;
        constexpr int ztorqorder = 1;
        constexpr float max_current = 5; //NEED TO CHECK
    }
    namespace battery{
        constexpr int pin = 32;
    }
    namespace fault{
        constexpr unsigned char mag_x = 1 << 0; 
        constexpr unsigned char mag_y = 1 << 1; 
        constexpr unsigned char mag_z = 1 << 2;
        constexpr unsigned char gyro_x = 1 << 3; 
        constexpr unsigned char gyro_y = 1 << 4; 
        constexpr unsigned char gyro_z = 1 << 5;
        constexpr unsigned char acc_x = 1 << 6; 
        constexpr unsigned char acc_y = 1 << 7;
    }
    namespace camera{
        //TODO
        constexpr int sleep_pin = 0;
    }
};

#endif

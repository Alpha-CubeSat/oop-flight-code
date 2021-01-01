#ifndef SFR_HPP_
#define SFR_HPP_

#include "mission_mode_type.enum"
#include "constants.hpp"

 
namespace sfr{
    namespace photoresistor{
        extern bool covered;
    } 
    namespace mission{
        extern mission_mode_type mode;
    }
    namespace burnwire{
        extern bool on;
    }
    namespace camera{
        extern bool photo_taken;
        extern bool take_photo;
    }
    namespace rockblock{
        extern uint8_t send_buffer[constants::rockblock::send_buffer_size];
        extern uint8_t receive_buffer[constants::rockblock::receive_buffer_size];
        extern size_t send_size;
        extern size_t receive_size;
    }
    namespace imu{
        extern float mag_x;
        extern float mag_y;
        extern float mag_z;
        extern float gyro_x;
        extern float gyro_y;
        extern float gyro_z;
        extern float acc_x;
        extern float acc_y;
        extern float acc_z;
        extern int gyr;
        extern int acc;
        extern int mag;
    }
    namespace temperature{
        extern float temp_c;
        extern float raw_temp;
    }
    namespace current{
        extern float solar_current;
    }
    namespace acs{
        extern bool detuble;
    }
    namespace battery{
        extern float voltage;
    }
};

#endif
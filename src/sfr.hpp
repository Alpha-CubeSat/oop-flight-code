#ifndef SFR_HPP_
#define SFR_HPP_

#include "mission_mode_type.enum"
#include "acs_mode_type.enum"
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
        extern uint8_t buffer[255];
        extern bool powered;
    }
    namespace rockblock{
        extern uint8_t fault_report[70];
        extern uint8_t old_fault_report[70];
        extern size_t fault_report_size;
        extern unsigned long last_downlink;
        extern unsigned long downlink_period;
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
        extern acs_mode_type mode;
        extern int current1;
        extern int current2;
        extern int current3;
    }
    namespace battery{
        extern float voltage;
    }
    namespace fault{
        extern bool is_fault;
        extern unsigned char imu_fault;
    }
};

#endif
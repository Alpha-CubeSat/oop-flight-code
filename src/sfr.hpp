#ifndef SFR_HPP_
#define SFR_HPP_

#include "mission_mode_type.enum"
#include "acs_mode_type.enum"
#include "rockblock_mode_type.enum"
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
        extern bool take_photo;
        extern uint8_t buffer[255];
        extern bool powered;
    }
    namespace rockblock{
        extern uint8_t fault_report[70];
        extern size_t fault_report_size;
        extern unsigned long last_downlink;
        extern unsigned long downlink_period;
        extern rockblock_mode_type mode;
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
        extern unsigned char fault_1;
        extern unsigned char fault_2;
        extern bool check_mag_x;
        extern bool check_mag_y;
        extern bool check_mag_z;
        extern bool check_gyro_x;
        extern bool check_gyro_y;
        extern bool check_gyro_z;
        extern bool check_acc_x;
        extern bool check_acc_y;
        extern bool check_acc_z;

        extern bool check_raw_temp;
        extern bool check_voltage;
    }
    namespace button{
        extern bool pressed;
    }
};

#endif
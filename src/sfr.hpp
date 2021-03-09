#ifndef SFR_HPP_
#define SFR_HPP_

/*#ifdef GIT
    #include "ArduinoFake.h"
#else
    #include "Arduino.h"
#endif*/

#include "Arduino.h"
#include "Modes/mission_mode_type.enum"
#include "Modes/acs_mode_type.enum"
#include "Modes/rockblock_mode_type.enum"
#include "Modes/burnwire_mode_type.enum"
#include "Modes/temp_mode_type.enum"
#include "Modes/fault_mode_type.enum"
#include "constants.hpp"

namespace sfr{
    namespace photoresistor{
        extern bool covered;
    } 
    namespace mission{
        extern mission_mode_type mode;
    }
    namespace burnwire{
        extern bool fire;
        extern burnwire_mode_type mode;
        extern int attempts;
        extern int start_time;
    }
    namespace camera{
        extern bool take_photo;
        extern bool turn_on;
        extern bool turn_off;
        extern bool powered;
        extern uint16_t current_serial;
        extern uint8_t fragment_number;
        extern uint8_t max_fragments;
        extern uint8_t data_length;
    }
    namespace rockblock{
        extern unsigned long last_downlink;
        extern unsigned long downlink_period;
        extern rockblock_mode_type mode;
        extern bool waiting_message;
        extern char buffer[constants::rockblock::buffer_size];
        extern uint8_t report[constants::rockblock::packet_size];
        extern int commas[constants::rockblock::num_commas];
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
    }
    namespace temperature{
        extern float temp_c;
        extern temp_mode_type mode;
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
        extern fault_mode_type mode;

        extern unsigned char fault_1;
        extern unsigned char fault_2;
        extern unsigned char fault_3;

        //FAULT 1
        extern bool check_mag_x;
        extern bool check_mag_y;
        extern bool check_mag_z;
        extern bool check_gyro_x;
        extern bool check_gyro_y;
        extern bool check_gyro_z;
        extern bool check_acc_x;
        extern bool check_acc_y;

        //FAULT 2
        extern bool check_acc_z;
        extern bool check_temp_c;
        extern bool check_solar_current;
        extern bool check_voltage;
    }
    namespace button{
        extern bool pressed;
    }
};

#endif
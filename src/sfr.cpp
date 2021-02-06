#include "sfr.hpp"
 
namespace sfr{
    namespace photoresistor{
        bool covered = true;
    } 
    namespace mission{
        mission_mode_type mode = mission_mode_type::standby;
    }
    namespace burnwire{
        bool fire = false;
        burnwire_mode_type mode = burnwire_mode_type::standby;
    }
    namespace camera{
        bool take_photo = false;
        uint8_t buffer[255] = {0};
        bool turn_on = false;
        bool turn_off = false;
        bool powered = false;
    }
    namespace rockblock{
        unsigned long last_downlink = 0;
        unsigned long downlink_period = constants::rockblock::two_hours;
        rockblock_mode_type mode = rockblock_mode_type::send_at;
    }
    namespace imu{
        float mag_x = 0.0;
        float mag_y = 0.0;
        float mag_z = 0.0;
        float gyro_x = 0.0;
        float gyro_y = 0.0;
        float gyro_z = 0.0;
        float acc_x = 0.0;
        float acc_y = 0.0;
        float acc_z = 0.0;
        int gyr = 0;
        int acc = 0;
        int mag = 0;
    }
    namespace temperature{
        float temp_c = 0.0;
    }
    namespace current{
        float solar_current = 0.0;
    }
    namespace acs{
        acs_mode_type mode = acs_mode_type::detumble;
        int current1 = 0;
        int current2 = 0;
        int current3 = 0;
    }
    namespace battery{
        float voltage = 0.0;
    }
    namespace fault{
        bool is_fault = false;
        unsigned char fault_1 = 0;
        unsigned char fault_2 = 0;
        unsigned char fault_3 = 0;
        bool check_mag_x = true;
        bool check_mag_y = true;
        bool check_mag_z = true;
        bool check_gyro_x = true;
        bool check_gyro_y = true;
        bool check_gyro_z = true;
        bool check_acc_x = true;
        bool check_acc_y = true;
        bool check_acc_z = true;

        bool check_temp_c = true;
        bool check_voltage = true;
    }
    namespace button{
        bool pressed = true;
    }
}
#include "sfr.hpp"
 
namespace sfr{
    namespace photoresistor{
        bool covered = true;
    } 
    namespace mission{
        mission_mode_type mode = mission_mode_type::standby;
    }
    namespace burnwire{
        bool on = false;
    }
    namespace camera{
        bool photo_taken = false;
        bool take_photo = false;
        uint8_t buffer[255] = {0};
        bool powered = true;
    }
    namespace rockblock{
        uint8_t fault_report[70] = {0};
        size_t fault_report_size = sizeof(fault_report);
        unsigned long last_downlink = 0;
        unsigned long downlink_period = constants::rockblock::two_hours;
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
        unsigned char imu_fault = 0;
    }
    namespace button{
        bool pressed = true;
    }
}
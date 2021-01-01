#include "sfr.hpp"
#include "constants.hpp"
 
namespace sfr{
    namespace photoresistor{
        bool covered = true;
    } 
    namespace mission{
        mission_mode_type mode = mission_mode_type::normal;
    }
    namespace burnwire{
        bool on = false;
    }
    namespace camera{
        bool photo_taken = false;
        bool take_photo = false;
    }
    namespace rockblock{
        uint8_t send_buffer[constants::rockblock::send_buffer_size] = {0};
        //uint8_t receive_buffer[constants::rockblock::receive_buffer_size] = {0};
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
        float raw_temp = 0.0;
    }
    namespace current{
        float solar_current = 0.0;
    }
    namespace acs{
        bool detuble = true;
    }
    namespace battery{
        float voltage = 0.0;
    }
}
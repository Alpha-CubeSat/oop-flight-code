#include "sfr.hpp"
#include "constants.hpp"
 
namespace sfr{
    namespace gps{
        float altitude = 0.0;
    }
    namespace photoresistor{
        bool covered = true;
    } 
    namespace mission{
        mission_mode_type mode = mission_mode_type::ascent;
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
}
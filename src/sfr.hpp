#ifndef SFR_HPP_
#define SFR_HPP_

#include "mission_mode_type.enum"
#include "constants.hpp"

 
namespace sfr{
    namespace gps{
        extern float altitude;
    }
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
};

#endif
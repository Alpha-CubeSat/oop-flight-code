#include "TemperatureControlTask.hpp"

TemperatureControlTask::TemperatureControlTask(unsigned int offset): TimedControlTask<void>(offset){}
        

void TemperatureControlTask::execute(){   
    if(sfr::temperature::mode == temp_mode_type::active && sfr::fault::check_temp_c == true){
        if(sfr::temperature::temp_c < 0 && sfr::camera::powered == true){
            #ifdef VERBOSE
            Serial.println("temperature monitor turned camera off");
            #endif
            sfr::camera::turn_off = true;
        }  else if (sfr::temperature::temp_c >= 0 && sfr::camera::powered == false) {
            #ifdef VERBOSE
            Serial.println("temperature monitor turned camera on");
            #endif
            sfr::camera::turn_on = true;
        }
    }
}
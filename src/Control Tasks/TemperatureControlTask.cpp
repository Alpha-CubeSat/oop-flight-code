#include "TemperatureControlTask.hpp"

TemperatureControlTask::TemperatureControlTask(){}
        

void TemperatureControlTask::execute(){   
    if(sfr::temperature::mode == temp_mode_type::active && sfr::fault::check_temp_c){
        if(sfr::temperature::temp_c < 0){
            sfr::camera::powered = false;
        }  else{
            sfr::camera::powered = true;
        }
    }
}
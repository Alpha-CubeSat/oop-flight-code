#include "BurnwireControlTask.hpp"

BurnwireControlTask::BurnwireControlTask(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::burnwire::first_pin, OUTPUT);
    pinMode(constants::burnwire::second_pin, OUTPUT);
}

void BurnwireControlTask::execute(){    
    burnwire_mode_type mode = sfr::burnwire::mode;

    switch(mode){
        case burnwire_mode_type::standby:
            { 
                if(sfr::burnwire::arm && sfr::mission::mode == mission_mode_type::deployment){
                    sfr::burnwire::mode = burnwire_mode_type::armed;
                    sfr::burnwire::start_time = millis();
                    sfr::burnwire::fire = false;
                } else{
                    transition_to_standby();
                }
                break;  
            }
        case burnwire_mode_type::armed:
            {
                if(sfr::mission::mode == mission_mode_type::deployment){
                    if(millis()-sfr::burnwire::start_time >= (uint32_t) sfr::burnwire::armed_time){
                        transition_to_standby();
                    }
                    if((sfr::burnwire::fire && sfr::temperature::in_sun && sfr::fault::check_temp_c) 
                    || (sfr::burnwire::fire && !sfr::fault::check_temp_c && sfr::fault::check_solar_current && sfr::current::in_sun)
                    || (sfr::burnwire::fire && !sfr::fault::check_solar_current && !sfr::fault::check_temp_c)){
                        sfr::burnwire::mode = burnwire_mode_type::fire;
                        sfr::camera::turn_on = true;
                    }
                    else{
                        digitalWrite(constants::burnwire::first_pin, LOW);
                        digitalWrite(constants::burnwire::second_pin, LOW);
                    }
                } else{
                    transition_to_standby();
                }
                break;
            }
        case burnwire_mode_type::fire:
            {
                if(sfr::mission::mode == mission_mode_type::deployment){
                    if(sfr::camera::powered){
                        dispatch_burn();
                    } 
                    else if(sfr::burnwire::camera_attempts >= constants::burnwire::camera_max_attempts){
                        dispatch_burn();
                    }
                    else{
                        sfr::burnwire::camera_attempts++;
                    }
                } else{
                    transition_to_standby();
                }
            break;
            }
        case burnwire_mode_type::burn:
            {
                if(sfr::mission::mode == mission_mode_type::deployment){
                    if(millis()-sfr::burnwire::start_time >= (uint32_t) sfr::burnwire::burn_time){
                        sfr::burnwire::mode = burnwire_mode_type::delay;
                        digitalWrite(constants::burnwire::first_pin, LOW);
                        digitalWrite(constants::burnwire::second_pin, LOW);
                        sfr::burnwire::start_time = millis();
                    }
                    else{
                        dispatch_burn();
                    }
                } else{
                    transition_to_standby();
                }
                break;
            }
        case burnwire_mode_type::delay:
            {
                if(sfr::mission::mode == mission_mode_type::deployment){
                    if(millis()-sfr::burnwire::start_time >= constants::burnwire::burn_wait){
                        dispatch_burn();
                    }
                    else{
                        digitalWrite(constants::burnwire::first_pin, LOW);
                        digitalWrite(constants::burnwire::second_pin, LOW);
                    }
                } else{
                    transition_to_standby();
                }
                break;
            }
    }
}

void BurnwireControlTask::dispatch_burn(){
    sfr::burnwire::camera_attempts = 0;
    sfr::burnwire::mode = burnwire_mode_type::burn;
    sfr::burnwire::start_time = millis();
    if(sfr::burnwire::attempts > constants::burnwire::max_attempts){
        transition_to_standby();
        sfr::fault::fault_3 = sfr::fault::fault_3 | constants::fault::burn_wire;
    }
    else{
        if(sfr::burnwire::attempts%2 == 0){
            digitalWrite(constants::burnwire::first_pin, HIGH);
            digitalWrite(constants::burnwire::second_pin, LOW);
        }
        else{
            digitalWrite(constants::burnwire::first_pin, LOW);
            digitalWrite(constants::burnwire::second_pin, HIGH);
        }
        sfr::burnwire::attempts++;
    }
}

void BurnwireControlTask::transition_to_standby(){
    sfr::burnwire::mode = burnwire_mode_type::standby;
    digitalWrite(constants::burnwire::first_pin, LOW);
    digitalWrite(constants::burnwire::second_pin, LOW);
    sfr::burnwire::fire = false;
    sfr::burnwire::arm = false;
    sfr::burnwire::attempts = 0;
}
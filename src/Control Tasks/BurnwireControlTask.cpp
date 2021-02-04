#include "BurnwireControlTask.hpp"

BurnwireControlTask::BurnwireControlTask(){
    pinMode(constants::burnwire::first_pin, OUTPUT);
    pinMode(constants::burnwire::second_pin, OUTPUT);
}

void BurnwireControlTask::execute(){    
    burnwire_mode_type mode = sfr::burnwire::mode;


    switch(mode){
            case burnwire_mode_type::standby:
                {
                    digitalWrite(constants::burnwire::first_pin, LOW);
                    digitalWrite(constants::burnwire::first_pin, LOW);
                    if(sfr::burnwire::fire){
                        sfr::burnwire::mode = burnwire_mode_type::first_burn;
                        start_time = millis();
                    }
                }
            case burnwire_mode_type::first_burn:
                {
                    digitalWrite(constants::burnwire::first_pin, HIGH);
                    if(millis()-start_time >= constants::burnwire::burn_time){
                        sfr::burnwire::mode = burnwire_mode_type::first_delay;
                        digitalWrite(constants::burnwire::first_pin, LOW);
                        start_time = millis();
                    }
                }
            case burnwire_mode_type::first_delay:
                {
                    if(millis()-start_time >= constants::burnwire::burn_wait){
                        sfr::burnwire::mode = burnwire_mode_type::second_burn;
                        start_time = millis();
                    }
                }
            case burnwire_mode_type::second_burn:
                {
                    digitalWrite(constants::burnwire::second_pin, HIGH);
                    if(millis()-start_time >= constants::burnwire::burn_time){
                        digitalWrite(constants::burnwire::first_pin, LOW);
                        attempts++;
                        if(attempts <= constants::burnwire::max_attempts){
                            sfr::burnwire::mode = burnwire_mode_type::second_delay;
                        }
                        else{
                            sfr::burnwire::mode = burnwire_mode_type::standby;
                            sfr::fault::fault_2 = sfr::fault::fault_1 | constants::fault::burn_wire;
                        }
                    }
                }
            case burnwire_mode_type::second_delay:
                {
                    if(millis()-start_time >= constants::burnwire::burn_wait){
                        sfr::burnwire::mode = burnwire_mode_type::first_burn;
                        start_time = millis();
                    }
                }
    }
}
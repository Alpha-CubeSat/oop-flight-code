#include "BurnwireControlTask.hpp"

BurnwireControlTask::BurnwireControlTask(){
    pinMode(constants::burnwire::first_pin, OUTPUT);
    pinMode(constants::burnwire::second_pin, OUTPUT);
}

void BurnwireControlTask::execute(){    
    if(sfr::burnwire::fire){
        if(start_burn_time == 0){
            digitalWrite(constants::burnwire::first_pin, HIGH);
            start_burn_time = millis();
        }
        else{
            if((millis()-start_burn_time) >= constants::burnwire::burn_time){
                digitalWrite(constants::burnwire::first_pin, LOW);
                digitalWrite(constants::burnwire::second_pin, LOW);
            }
        }  
    }
    if(!sfr::button::pressed){
        sfr::burnwire::fire = false;
    }
}
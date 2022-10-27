#include "BurnwireControlTask.hpp"

BurnwireControlTask::BurnwireControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void BurnwireControlTask::execute()
{
    mode = static_cast<burnwire_mode_type>(sfr::burnwire::mode.get());

    switch(mode){
    case burnwire_mode_type::burn:  {
        if (sfr::mission::current_mode == sfr::mission::mandatoryBurns){
            if (millis() - sfr::burnwire::mandatoryburn_start_time >= (uint32_t)sfr::burnwire::burn_time) {
                mode = burnwire_mode_type::delay;
                Pins::setPinState(constants::burnwire::first_pin, LOW);
                Pins::setPinState(constants::burnwire::second_pin, LOW);
                sfr::burnwire::mandatoryburn_start_time = millis();
            } else {
                mandatory_dispatch_burn();
            }
        } else if (sfr::mission::current_mode == sfr::mission::regularBurns){
            if (millis() - sfr::burnwire::regularburn_start_time >= (uint32_t)sfr::burnwire::burn_time) {
                mode = burnwire_mode_type::delay;
                Pins::setPinState(constants::burnwire::first_pin, LOW);
                Pins::setPinState(constants::burnwire::second_pin, LOW);
                sfr::burnwire::regularburn_start_time = millis();
            } else {
                regular_dispatch_burn();
            }
        }else{
            //No case to handle here
        }
        break;

        }
    case burnwire_mode_type::delay:  {
        if (sfr::mission::current_mode == sfr::mission::mandatoryBurns){
            if (millis() - sfr::burnwire::mandatoryburn_start_time >= (uint32_t)sfr::burnwire::burn_wait) {
                mandatory_dispatch_burn();
            }else {
                Pins::setPinState(constants::burnwire::first_pin, LOW);
                Pins::setPinState(constants::burnwire::second_pin, LOW);
            }
        } else if (sfr::mission::current_mode == sfr::mission::regularBurns){
            if (millis() - sfr::burnwire::regularburn_start_time >= (uint32_t)sfr::burnwire::burn_wait) {
                regular_dispatch_burn();
            }else{
                Pins::setPinState(constants::burnwire::first_pin, LOW);
                Pins::setPinState(constants::burnwire::second_pin, LOW);
            } 
            }else {
               //No case to handle here
            }
        }
        break;
        }
    } 

void BurnwireControlTask::mandatory_dispatch_burn(){
    // sfr::burnwire::camera_attempts = 0;
    sfr::burnwire::mode = (uint16_t)burnwire_mode_type::burn;
    sfr::burnwire::mandatoryburn_start_time = millis();
    if (sfr::burnwire::mandatory_attempts % 2 == 0) {
        Pins::setPinState(constants::burnwire::first_pin, HIGH);
        Pins::setPinState(constants::burnwire::second_pin, LOW);
    } else {
        Pins::setPinState(constants::burnwire::first_pin, LOW);
        Pins::setPinState(constants::burnwire::second_pin, HIGH);
    }
        sfr::burnwire::mandatory_attempts++;
}

void BurnwireControlTask::regular_dispatch_burn(){
    // sfr::burnwire::camera_attempts = 0;
    sfr::burnwire::mode = (uint16_t)burnwire_mode_type::burn;
    sfr::burnwire::regularburn_start_time = millis();
    if (sfr::burnwire::regular_attempts % 2 == 0) {
        Pins::setPinState(constants::burnwire::first_pin, HIGH);
        Pins::setPinState(constants::burnwire::second_pin, LOW);
    } else {
        Pins::setPinState(constants::burnwire::first_pin, LOW);
        Pins::setPinState(constants::burnwire::second_pin, HIGH);
    }
        sfr::burnwire::regular_attempts++;
}


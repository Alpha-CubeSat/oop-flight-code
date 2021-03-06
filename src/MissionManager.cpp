#include "MissionManager.hpp"

MissionManager::MissionManager(){}

void MissionManager::execute(){
    mission_mode_type mode = sfr::mission::mode;

    switch(mode){
        case mission_mode_type::low_power:
            dispatch_low_power();
            break;
        case mission_mode_type::safe:
            dispatch_safe();
            break;
        case mission_mode_type::standby:
            dispatch_standby();
            break;
        case mission_mode_type::deployment:
            dispatch_deployment();
            break;
    }
}

void MissionManager::dispatch_standby(){
    if(sfr::battery::voltage < 3.75 && sfr::fault::check_voltage){
        sfr::mission::mode = mission_mode_type::low_power;
        transition_to_low_power();
    }
}

void MissionManager::dispatch_safe(){}

void MissionManager::dispatch_low_power(){
    if(sfr::battery::voltage > 3.9 and sfr::fault::check_voltage){
        sfr::mission::mode = mission_mode_type::standby;
        transition_to_standby();
    }    
}

void MissionManager::dispatch_deployment(){
    if(!sfr::button::pressed){
        sfr::mission::mode = mission_mode_type::standby;
        sfr::burnwire::fire = false;
        sfr::camera::take_photo = true;
        sfr::mission::mode = mission_mode_type::standby;
        transition_to_standby();
    }
}

void MissionManager::transition_to_standby(){
    sfr::fault::mode = fault_mode_type::active;
    sfr::acs::mode = acs_mode_type::point;
    sfr::temperature::mode = temp_mode_type::active;
    sfr::rockblock::downlink_period = constants::rockblock::ten_minutes;
}

void MissionManager::transition_to_safe(){
    sfr::acs::mode = acs_mode_type::off;
    sfr::rockblock::downlink_period = constants::rockblock::ten_minutes;
    sfr::temperature::mode = temp_mode_type::active;
    sfr::fault::mode = fault_mode_type::inactive;
}

void MissionManager::transition_to_low_power(){
    sfr::fault::mode = fault_mode_type::active;
    sfr::acs::mode = acs_mode_type::off;
    sfr::rockblock::downlink_period= constants::rockblock::two_hours;
    sfr::temperature::mode = temp_mode_type::inactive;
}

void MissionManager::transition_to_deployment(){
    sfr::acs::mode = acs_mode_type::off;
    sfr::camera::powered = true;
    sfr::temperature::mode = temp_mode_type::inactive;
}


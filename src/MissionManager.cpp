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
    if(sfr::fault::is_fault){
        sfr::mission::mode = mission_mode_type::safe;
        dispatch_safe();
    }
    if(sfr::battery::voltage < 3.75 && sfr::fault::check_voltage){
        sfr::mission::mode == mission_mode_type::low_power;
        dispatch_low_power();
    }
    //TODO: get these values
    if(sfr::fault::check_acc_x and sfr::fault::check_acc_y){
        if(sfr::imu::acc_x < 0 and sfr::imu::acc_y < 0){
            sfr::acs::mode = acs_mode_type::point;
        }
        else{
            sfr::acs::mode = acs_mode_type::detumble;
        }
    }
    sfr::rockblock::downlink_period = constants::rockblock::ten_minutes;
    //TODO get this val
    if(sfr::fault::check_raw_temp){
        if(sfr::temperature::raw_temp < 0){
            sfr::camera::powered = false;
        }  else{
            sfr::camera::powered = true;
        }
    }
}

void MissionManager::dispatch_safe(){
    sfr::acs::mode = acs_mode_type::off;
    sfr::rockblock::downlink_period = constants::rockblock::ten_minutes;
    //TODO get this val
    if(sfr::fault::check_raw_temp){
        if(sfr::temperature::raw_temp < 0){
            sfr::camera::powered = false;
        }  else{
            sfr::camera::powered = true;
        }
    }
}

void MissionManager::dispatch_low_power(){
    if(sfr::fault::is_fault == true){
        sfr::mission::mode == mission_mode_type::safe;
        dispatch_safe();
    }
    if(sfr::battery::voltage > 3.9 and sfr::fault::check_voltage){
        sfr::mission::mode == mission_mode_type::standby;
        dispatch_standby();
    }
    sfr::acs::mode = acs_mode_type::off;
    sfr::rockblock::downlink_period= constants::rockblock::two_hours;
    //TODO get this val
    if(sfr::fault::check_raw_temp){
        if(sfr::temperature::raw_temp < 0){
            sfr::camera::powered = false;
        }  else{
            sfr::camera::powered = true;
        }
    }
}

void MissionManager::dispatch_deployment(){
    sfr::acs::mode = acs_mode_type::off;
    sfr::camera::powered = true;
    if(sfr::button::pressed = false){
        sfr::mission::mode = mission_mode_type::standby;
    }
}


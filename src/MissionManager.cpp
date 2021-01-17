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
    if(sfr::fault::is_fault == true){
        sfr::mission::mode == mission_mode_type::safe;
    }
    if(sfr::battery::voltage < 3.75){
        sfr::mission::mode == mission_mode_type::low_power;
    }
    //TODO: get these values
    if(sfr::imu::acc_x < 0 and sfr::imu::acc_y < 0){
        sfr::acs::mode = acs_mode_type::point;
    }
    else{
        sfr::acs::mode = acs_mode_type::detumble;
    }

}

void MissionManager::dispatch_safe(){
    sfr::acs::mode = acs_mode_type::off;
}

void MissionManager::dispatch_low_power(){
    sfr::acs::mode = acs_mode_type::off;
}

void MissionManager::dispatch_deployment(){
    sfr::acs::mode = acs_mode_type::off;
}


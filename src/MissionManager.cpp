#include "MissionManager.hpp"

MissionManager::MissionManager(){}

void MissionManager::execute(){
    mission_mode_type mode = sfr::mission::mode;

    switch(mode){
        case mission_mode_type::normal:
            dispatch_normal();
            break;
        case mission_mode_type::detumble:
            dispatch_detumble();
            break;
        case mission_mode_type::safe_hold:
            dispatch_safe_hold();
            break;
        case mission_mode_type::low_power:
            dispatch_low_power();
            break;
        case mission_mode_type::deploy:
            dispatch_deploy();
    }
}

void MissionManager::dispatch_normal(){}

void MissionManager::dispatch_detumble(){}

void MissionManager::dispatch_safe_hold(){}

void MissionManager::dispatch_low_power(){}

void MissionManager::dispatch_deploy(){}


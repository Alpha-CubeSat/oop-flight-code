#include "MissionManager.hpp"

MissionManager::MissionManager(){}

void MissionManager::execute(){
    mission_mode_type mode = sfr::mission::mode;

    switch(mode){
        case mission_mode_type::ascent:
            dispatch_ascent();
            break;
        case mission_mode_type::deploy_lightsail:
            dispatch_deploy_lightsail();
            break;
        case mission_mode_type::taking_photo:
            dispatch_take_photo();
            break;
        case mission_mode_type::descent:
            dispatch_descent();
            break;
    }
}

void MissionManager::dispatch_ascent(){
    if(sfr::gps::altitude > constants::gps::mand_deploy){
        sfr::mission::mode = mission_mode_type::deploy_lightsail;
    }
}

void MissionManager::dispatch_deploy_lightsail(){
    if(sfr::photoresistor::covered){
        sfr::burnwire::on = true;
    }
    else{
        sfr::burnwire::on = false;
        sfr::mission::mode = mission_mode_type::taking_photo;
    }
}

void MissionManager::dispatch_take_photo(){
    if(!sfr::camera::photo_taken){
        sfr::camera::take_photo = true;
    }
    else{
        sfr::camera::take_photo = false;
        sfr::mission::mode = mission_mode_type::descent;
    }
}

void MissionManager::dispatch_descent(){}
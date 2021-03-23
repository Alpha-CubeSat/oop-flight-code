#include "CommandMonitor.hpp"

CommandMonitor::CommandMonitor(){}

void CommandMonitor::execute(){
    if(sfr::rockblock::waiting_command){
        //transition to mission mode
        //change rockblock downlink frequency

        if(sfr::rockblock::opcode[0] == '0' && sfr::rockblock::opcode[1] == '0'){
            dispatch_change_mission_mode();
        }
        else if(sfr::rockblock::opcode[0] == '1' || sfr::rockblock::opcode[0] == '2'){
            dispatch_change_fault_checks();
        }
        sfr::rockblock::waiting_command = false;
    }

}

void CommandMonitor::dispatch_change_mission_mode(){
    switch(sfr::rockblock::argument[1]){
        case '0':
            sfr::mission::mode = mission_mode_type::low_power;
            MissionManager::transition_to_low_power();
            break;
        case '1':
            sfr::mission::mode = mission_mode_type::deployment;
            MissionManager::transition_to_deployment();
            break;
        case '2':
            sfr::mission::mode = mission_mode_type::standby;
            MissionManager::transition_to_standby();
            break;
        case '3':
            sfr::mission::mode = mission_mode_type::safe;
            MissionManager::transition_to_safe();
            break;
    }
}

void CommandMonitor::dispatch_change_fault_checks(){
    /*switch(sfr::rockblock::opcode[0]){
        case '1':
            switch(sfr::rockblock::opcode[1]){
                case '0':
                    if(sfr::rockblock::argument[1] == 0){
                        sfr::fault::check_mag_x = false;
                    }else{
                        sfr::fault::check_mag_x = true;
                    }
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
            }
        case '2':
            switch(sfr::rockblock::opcode[1]){
            }

    }*/

}



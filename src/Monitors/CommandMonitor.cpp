#include "CommandMonitor.hpp"

CommandMonitor::CommandMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void CommandMonitor::execute(){
    if(sfr::rockblock::waiting_command){

        switch(sfr::rockblock::f_opcode){
            int mission_mode = get_decimal_opcode(constants::rockblock::mission_mode[0], constants::rockblock::mission_mode[1]);
            case mission_mode:
                dispatch_change_mission_mode();
                break;
            case 1:
                dispatch_change_fault_checks();
            case 2
        }
        if(sfr::rockblock::opcode[0] == '0' && sfr::rockblock::opcode[1] == '0'){
            
        }
        else if(sfr::rockblock::opcode[0] == '1' || sfr::rockblock::opcode[0] == '2'){
            
        }
        sfr::rockblock::waiting_command = false;
    }

}

void CommandMonitor::dispatch_change_mission_mode(){
    /*switch(sfr::rockblock::argument[1]){
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
    }*/
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

int CommandMonitor::get_decimal_opcode(uint8_t hex_opcode_byte_one, uint8_t hex_opcode_byte_two){
    uint16_t c_opcode = hex_opcode_byte_one | (hex_opcode_byte_two << 8);
    std::stringstream ss_opcode;
    ss_opcode << c_opcode;
    int f_opcode;
    ss_opcode >> f_opcode;
    return f_opcode;
}

int CommandMonitor::get_decimal_arg(uint8_t hex_arg_byte_one, uint8_t hex_arg_byte_two, uint8_t hex_arg_byte_three, uint8_t hex_arg_byte_four){
    uint32_t c_arg = sfr::rockblock::arg_2[0] | (sfr::rockblock::arg_2[1] << 8) | (sfr::rockblock::arg_2[2] << 16) | (sfr::rockblock::arg_2[3] << 24);
    std::stringstream ss_arg;
    ss_arg << c_arg;
    int f_arg;
    ss_arg >> f_arg;
    return f_arg;
}



#include "CommandMonitor.hpp"

CommandMonitor::CommandMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void CommandMonitor::execute(){
    if(sfr::rockblock::waiting_command){
        if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::mission_mode[0], constants::rockblock::mission_mode[1])){
            dispatch_change_mission_mode();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::burnwire_arm[0], constants::rockblock::burnwire_arm[1])){
            dispatch_change_true_false(sfr::burnwire::arm);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::burnwire_fire[0], constants::rockblock::burnwire_fire[1])){
            dispatch_change_true_false(sfr::burnwire::fire);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::camera_take_photo[0], constants::rockblock::camera_take_photo[1])){
            dispatch_change_true_false(sfr::camera::take_photo);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::temperature_mode[0], constants::rockblock::temperature_mode[1])){
            dispatch_change_temperature_mode();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::acs_mode[0], constants::rockblock::acs_mode[1])){
            dispatch_change_acs_mode();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_mode[0], constants::rockblock::fault_mode[1])){
            dispatch_change_fault_mode();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_mag_x[0], constants::rockblock::fault_check_mag_x[1])){
            dispatch_change_true_false(sfr::fault::check_mag_x);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_mag_y[0], constants::rockblock::fault_check_mag_y[1])){
            dispatch_change_true_false(sfr::fault::check_mag_y);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_mag_z[0], constants::rockblock::fault_check_mag_z[1])){
            dispatch_change_true_false(sfr::fault::check_mag_z);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_gyro_x[0], constants::rockblock::fault_check_gyro_x[1])){
            dispatch_change_true_false(sfr::fault::check_gyro_x);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_gyro_y[0], constants::rockblock::fault_check_gyro_y[1])){
            dispatch_change_true_false(sfr::fault::check_gyro_y);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_gyro_z[0], constants::rockblock::fault_check_gyro_z[1])){
            dispatch_change_true_false(sfr::fault::check_gyro_z);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_temp_c[0], constants::rockblock::fault_check_temp_c[1])){
            dispatch_change_true_false(sfr::fault::check_temp_c);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_solar_current[0], constants::rockblock::fault_check_solar_current[1])){
            dispatch_change_true_false(sfr::fault::check_solar_current);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_voltage[0], constants::rockblock::fault_check_voltage[1])){
            dispatch_change_true_false(sfr::fault::check_voltage);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::request_image_fragment[0], constants::rockblock::request_image_fragment[1])){
            dispatch_request_image_fragment();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::rockblock_downlink_period[0], constants::rockblock::rockblock_downlink_period[1])){
            dispatch_change_rockblock_downlink_period();
        }
        else{
            sfr::rockblock::waiting_command = false;
        }
    }
}

void CommandMonitor::dispatch_change_mission_mode(){
    if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::deployment[0], constants::rockblock::deployment[1], constants::rockblock::deployment[2], constants::rockblock::deployment[3])){
        MissionManager::transition_to_deployment();
    }
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::standby[0], constants::rockblock::standby[1], constants::rockblock::standby[2], constants::rockblock::standby[3])){
        MissionManager::transition_to_standby();
    }
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::safe[0], constants::rockblock::safe[1], constants::rockblock::safe[2], constants::rockblock::safe[3])){
        MissionManager::transition_to_safe();
    }
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::low_power[0], constants::rockblock::low_power[1], constants::rockblock::low_power[2], constants::rockblock::low_power[3])){
        MissionManager::transition_to_low_power();
    }
    sfr::rockblock::waiting_command = false;
}

void CommandMonitor::dispatch_change_true_false(bool value){
    value = sfr::rockblock::f_arg_1;
    sfr::rockblock::waiting_command = false;
}

void CommandMonitor::dispatch_change_temperature_mode(){
    if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::active[0],constants::rockblock::active[1],constants::rockblock::active[2],constants::rockblock::active[3])){
        sfr::temperature::mode = temp_mode_type::active;
    } 
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::inactive[0],constants::rockblock::inactive[1],constants::rockblock::inactive[2],constants::rockblock::inactive[3])){
        sfr::temperature::mode = temp_mode_type::inactive;
    }
    sfr::rockblock::waiting_command = false;
}
void CommandMonitor::dispatch_change_acs_mode(){
    if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::detumbling[0],constants::rockblock::detumbling[1],constants::rockblock::detumbling[2],constants::rockblock::detumbling[3])){
        sfr::acs::mode = acs_mode_type::detumble;
    } 
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::pointing[0],constants::rockblock::pointing[1],constants::rockblock::pointing[2],constants::rockblock::pointing[3])){
        sfr::acs::mode = acs_mode_type::point;
    }
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::off[0],constants::rockblock::off[1],constants::rockblock::off[2],constants::rockblock::off[3])){
        sfr::acs::mode = acs_mode_type::off;
    }
    sfr::rockblock::waiting_command = false;
}

void CommandMonitor::dispatch_change_fault_mode(){
    if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::active[0],constants::rockblock::active[1],constants::rockblock::active[2],constants::rockblock::active[3])){
        sfr::temperature::mode = temp_mode_type::active;
    } 
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::inactive[0],constants::rockblock::inactive[1],constants::rockblock::inactive[2],constants::rockblock::inactive[3])){
        sfr::temperature::mode = temp_mode_type::inactive;
    }
    sfr::rockblock::waiting_command = false;
}

void CommandMonitor::dispatch_request_image_fragment(){
    //TODO read Kinjal's code
    sfr::rockblock::waiting_command = false;
}

void CommandMonitor::dispatch_change_rockblock_downlink_period(){
    if(sfr::rockblock::f_arg_1 < constants::rockblock::max_downlink_period && sfr::rockblock::f_arg_1 > constants::rockblock::min_downlink_period){
        sfr::rockblock::downlink_period = sfr::rockblock::f_arg_1;
    }
    sfr::rockblock::waiting_command = false;
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



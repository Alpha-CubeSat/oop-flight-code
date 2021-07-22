#include "CommandMonitor.hpp"

CommandMonitor::CommandMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void CommandMonitor::execute(){
    if(sfr::rockblock::waiting_command){
        if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::mission_mode)){
            dispatch_change_mission_mode();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::burnwire_arm)){
            dispatch_change_true_false(sfr::burnwire::arm);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::burnwire_fire)){
            dispatch_change_true_false(sfr::burnwire::fire);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::burnwire_time)){
            dispatch_change_burnwire_time();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::burnwire_timeout)){
            dispatch_change_burnwire_timeout();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::rockblock_downlink_period)){
            dispatch_change_rockblock_downlink_period();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::request_image_fragment)){
            dispatch_request_image_fragment();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::camera_take_photo)){
            
            dispatch_change_true_false(sfr::camera::take_photo);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::temperature_mode)){
            dispatch_change_temperature_mode();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::acs_mode)){
            dispatch_change_acs_mode();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_mode)){
            dispatch_change_fault_mode();
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_mag_x)){
            dispatch_change_true_false(sfr::fault::check_mag_x);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_mag_y)){
            dispatch_change_true_false(sfr::fault::check_mag_y);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_mag_z)){
            dispatch_change_true_false(sfr::fault::check_mag_z);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_gyro_x)){
            dispatch_change_true_false(sfr::fault::check_gyro_x);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_gyro_y)){
            dispatch_change_true_false(sfr::fault::check_gyro_y);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_gyro_z)){
            dispatch_change_true_false(sfr::fault::check_gyro_z);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_temp_c)){
            dispatch_change_true_false(sfr::fault::check_temp_c);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_solar_current)){
            dispatch_change_true_false(sfr::fault::check_solar_current);
        }
        else if(sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_voltage)){
            dispatch_change_true_false(sfr::fault::check_voltage);
        }

        sfr::rockblock::waiting_command = false;
    }
}

void CommandMonitor::dispatch_change_mission_mode(){
    if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::deployment)){
        MissionManager::transition_to_deployment();
    }
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::standby)){
        MissionManager::transition_to_standby();
    }
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::safe)){
        MissionManager::transition_to_safe();
    }
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::low_power)){
        MissionManager::transition_to_low_power();
    }
}

void CommandMonitor::dispatch_change_true_false(bool& value){
    value = sfr::rockblock::f_arg_1;
}

void CommandMonitor::dispatch_change_temperature_mode(){
    if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::active)){
        sfr::temperature::mode = temp_mode_type::active;
    } 
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::inactive)){
        sfr::temperature::mode = temp_mode_type::inactive;
    }
}
void CommandMonitor::dispatch_change_acs_mode(){
    if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::full)){
        sfr::acs::mode = acs_mode_type::full;
    } 
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::simple)){
        sfr::acs::mode = acs_mode_type::simple;
    }
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::off)){
        sfr::acs::mode = acs_mode_type::off;
    }
}

void CommandMonitor::dispatch_change_fault_mode(){
    if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::active)){
        sfr::fault::mode = fault_mode_type::active;
    } 
    else if(sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::inactive)){
        sfr::fault::mode = fault_mode_type::inactive;
    }
}

void CommandMonitor::dispatch_request_image_fragment(){
    if(sfr::rockblock::f_arg_2 < sfr::rockblock::camera_max_fragments[sfr::rockblock::f_arg_1]){
        sfr::camera::fragment_requested = true;
        sfr::camera::fragment_number_requested = sfr::rockblock::f_arg_2;
        sfr::camera::serial_requested = sfr::rockblock::f_arg_1;

        #ifdef VERBOSE
        Serial.print("Fragment requested: ");
        Serial.println(sfr::camera::fragment_number_requested);
        Serial.print("Serial requested: ");
        Serial.println(sfr::camera::serial_requested);
        #endif
    }  
}

void CommandMonitor::dispatch_change_rockblock_downlink_period(){
    if(sfr::rockblock::f_arg_1 < constants::rockblock::max_downlink_period && sfr::rockblock::f_arg_1 > constants::rockblock::min_downlink_period){
        sfr::rockblock::downlink_period = sfr::rockblock::f_arg_1;
    } 
}

void CommandMonitor::dispatch_change_burnwire_time() {
    if(sfr::rockblock::f_arg_1 < constants::burnwire::max_burnwire_time && sfr::rockblock::f_arg_1 > constants::burnwire::min_burnwire_time){
        sfr::burnwire::burn_time = sfr::rockblock::f_arg_1;
    }
}

void CommandMonitor::dispatch_change_burnwire_timeout() {
    if(sfr::rockblock::f_arg_1 < constants::burnwire::max_armed_time && sfr::rockblock::f_arg_1 > constants::burnwire::min_armed_time){
        sfr::burnwire::armed_time = sfr::rockblock::f_arg_1;
    }
}

uint16_t CommandMonitor::get_decimal_opcode(const uint8_t* hex_opcode_bytes){
    return (hex_opcode_bytes[1] << 8) | (hex_opcode_bytes[0]);
}

uint32_t CommandMonitor::get_decimal_arg(const uint8_t* hex_arg_bytes){
    return (hex_arg_bytes[3] << 24) | (hex_arg_bytes[2]) << 16 | (hex_arg_bytes[1] << 8) | (hex_arg_bytes[0]);
}
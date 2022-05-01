#include "CommandMonitor.hpp"
#include "NormalReportMonitor.hpp"

CommandMonitor::CommandMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void CommandMonitor::execute()
{
    if (sfr::rockblock::waiting_command) {
        bool mag_x_average_isValid = sfr::imu::mag_x_average->is_valid();
        bool mag_y_average_isValid = sfr::imu::mag_y_average->is_valid();
        bool mag_z_average_isValid = sfr::imu::mag_z_average->is_valid();
        bool gyro_x_average_isValid = sfr::imu::gyro_x_average->is_valid();
        bool gyro_y_average_isValid = sfr::imu::gyro_y_average->is_valid();
        bool gyro_z_average_isValid = sfr::imu::gyro_z_average->is_valid();
        bool temp_c_average_isValid = sfr::temperature::temp_c_average->is_valid();
        bool voltage_average_isValid = sfr::battery::voltage_average->is_valid();
        bool solar_current_average_isValid = sfr::current::solar_current_average->is_valid();
        NormalReportMonitor::commands_received.push(sfr::rockblock::f_opcode & 0xFF);          // Add byte 0 of opcode to the queue
        NormalReportMonitor::commands_received.push((sfr::rockblock::f_opcode & 0xFF00) >> 8); // Add byte 1 of opcode to the queue
        if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::mission_mode)) {
            dispatch_change_mission_mode();
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::burnwire_arm)) {
            dispatch_change_true_false(sfr::burnwire::arm);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::burnwire_fire)) {
            dispatch_change_true_false(sfr::burnwire::fire);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::burnwire_time)) {
            dispatch_change_burnwire_time();
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::burnwire_timeout)) {
            dispatch_change_burnwire_timeout();
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::rockblock_downlink_period)) {
            dispatch_change_rockblock_downlink_period();
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::request_image_fragment)) {
            dispatch_request_image_fragment();
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::camera_take_photo)) {
            dispatch_change_true_false(sfr::camera::take_photo);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::acs_mode)) {
            dispatch_change_acs_mode();
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_mode)) {
            dispatch_change_fault_mode();
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_mag_x)) {
            dispatch_change_true_false(mag_x_average_isValid);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_mag_y)) {
            dispatch_change_true_false(mag_y_average_isValid);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_mag_z)) {
            dispatch_change_true_false(mag_z_average_isValid);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_gyro_x)) {
            dispatch_change_true_false(gyro_x_average_isValid);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_gyro_y)) {
            dispatch_change_true_false(gyro_y_average_isValid);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_gyro_z)) {
            dispatch_change_true_false(gyro_z_average_isValid);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_temp_c)) {
            dispatch_change_true_false(temp_c_average_isValid);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_solar_current)) {
            dispatch_change_true_false(voltage_average_isValid);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::fault_check_voltage)) {
            dispatch_change_true_false(solar_current_average_isValid);
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::change_simplified_acs)) {
            dispatch_change_simplified_acs();
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::camera_turn_on)) {
            sfr::camera::turn_on = true;
        } else if (sfr::rockblock::f_opcode == get_decimal_opcode(constants::rockblock::camera_turn_off)) {
            sfr::camera::turn_off = true;
        }
        sfr::rockblock::waiting_command = false;
    }
}

void CommandMonitor::dispatch_change_mission_mode()
{
    /*if (sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::initialization)) {
        MissionManager::transition_to_initialization();
    } else if (sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::deployment)) {
        MissionManager::transition_to_deployment();
    } else if (sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::standby)) {
        MissionManager::transition_to_standby();
    } else if (sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::safe)) {
        MissionManager::transition_to_safe();
    } else if (sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::low_power)) {
        MissionManager::transition_to_low_power();
    }*/
}

void CommandMonitor::dispatch_change_true_false(bool &value)
{
    value = sfr::rockblock::f_arg_1;
}

void CommandMonitor::dispatch_change_acs_mode()
{
    /*if (sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::full)) {
        sfr::acs::mode = acs_mode_type::full;
    } else if (sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::simple)) {
        sfr::acs::mode = acs_mode_type::simple;
    } else if (sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::off)) {
        sfr::acs::mode = acs_mode_type::off;
    }*/
}

void CommandMonitor::dispatch_change_fault_mode()
{
    if (sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::active)) {
        sfr::fault::mode = fault_mode_type::active;
    } else if (sfr::rockblock::f_arg_1 == get_decimal_arg(constants::rockblock::inactive)) {
        sfr::fault::mode = fault_mode_type::inactive;
    }
}

void CommandMonitor::dispatch_request_image_fragment()
{
    if (sfr::rockblock::f_arg_2 < sfr::rockblock::camera_max_fragments[sfr::rockblock::f_arg_1]) {
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

void CommandMonitor::dispatch_change_rockblock_downlink_period()
{
    if (sfr::rockblock::f_arg_1 < constants::rockblock::max_downlink_period && sfr::rockblock::f_arg_1 > constants::rockblock::min_downlink_period) {
        sfr::rockblock::downlink_period = sfr::rockblock::f_arg_1;
    }
}

void CommandMonitor::dispatch_change_burnwire_time()
{
    if (sfr::rockblock::f_arg_1 < constants::burnwire::max_burnwire_time && sfr::rockblock::f_arg_1 > constants::burnwire::min_burnwire_time) {
        sfr::burnwire::burn_time = sfr::rockblock::f_arg_1;
    }
}

void CommandMonitor::dispatch_change_burnwire_timeout()
{
    if (sfr::rockblock::f_arg_1 < constants::burnwire::max_armed_time && sfr::rockblock::f_arg_1 > constants::burnwire::min_armed_time) {
        sfr::burnwire::armed_time = sfr::rockblock::f_arg_1;
    }
}

void CommandMonitor::dispatch_change_simplified_acs()
{
    sfr::acs::mag = (simple_acs_type)(int)sfr::rockblock::f_arg_1;
}

uint16_t CommandMonitor::get_decimal_opcode(const uint8_t *hex_opcode_bytes)
{
    return (hex_opcode_bytes[1] << 8) | (hex_opcode_bytes[0]);
}

uint32_t CommandMonitor::get_decimal_arg(const uint8_t *hex_arg_bytes)
{
    return (hex_arg_bytes[3] << 24) | (hex_arg_bytes[2]) << 16 | (hex_arg_bytes[1] << 8) | (hex_arg_bytes[0]);
}
#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <cstddef>
#include <stdint.h>

namespace constants {
    namespace time {
        constexpr unsigned long one_second = 1000;
        constexpr unsigned long one_minute = 60 * one_second;
        constexpr unsigned long one_hour = 60 * one_minute;
        constexpr unsigned long one_day = 24 * one_hour;

        constexpr unsigned long half_second = one_second / 2;
        constexpr unsigned long ten_minutes = 10 * one_minute;
        constexpr unsigned long two_hours = 2 * one_hour;
        constexpr unsigned long two_days = 2 * one_day;
    } // namespace time
    namespace sensor {
        //window size for moving kernel fileter (rn not the exact size)
        constexpr int collect = 5;
    } // namespace sensor
    namespace photoresistor {
        constexpr int pin = 38;
        constexpr int light_val = 200;
        constexpr int min_val = 0;
        constexpr int max_val = 1023;
    } // namespace photoresistor
    namespace burnwire {
        constexpr int first_pin = 14;
        constexpr int second_pin = 15;
        constexpr int burn_wait = 1000;
        constexpr int max_attempts = 10;
        constexpr int camera_max_attempts = 50;
        constexpr int min_burnwire_time = 0;
        constexpr int max_burnwire_time = 60000;
        constexpr int min_armed_time = 0;
        constexpr int max_armed_time = 86400000;
    } // namespace burnwire
    namespace rockblock {
        constexpr int sleep_pin = 19;

        constexpr int min_sleep_period = 2 * time::one_minute;

        constexpr int min_downlink_period = time::one_second;
        constexpr int max_downlink_period = time::two_days;

        constexpr int baud = 19200;
        constexpr size_t buffer_size = 63;
        constexpr size_t packet_size = 70;
        constexpr size_t num_commas = 5;
        constexpr size_t max_iter = 200;
        constexpr size_t num_commands = 41;
        constexpr size_t opcode_len = 2;
        constexpr size_t arg1_len = 4;
        constexpr size_t arg2_len = 4;
        constexpr size_t command_len = opcode_len + arg1_len + arg2_len;
        constexpr size_t max_conseq_read = 3;

        constexpr int num_initial_downlinks = 2;
        constexpr uint8_t start_of_normal_downlink_flag = 99;
        constexpr uint8_t end_of_normal_downlink_flag1 = 254;
        constexpr uint8_t end_of_normal_downlink_flag2 = 255;

        constexpr uint8_t mission_mode[opcode_len] = {0x00, 0x00};
        constexpr uint8_t burnwire_arm[opcode_len] = {0x01, 0x00};
        constexpr uint8_t burnwire_fire[opcode_len] = {0x02, 0x00};
        constexpr uint8_t burnwire_time[opcode_len] = {0x03, 0x00};
        constexpr uint8_t burnwire_timeout[opcode_len] = {0x04, 0x00};
        constexpr uint8_t rockblock_downlink_period[opcode_len] = {0x05, 0x00};
        constexpr uint8_t request_image_fragment[opcode_len] = {0x06, 0x00};
        constexpr uint8_t camera_take_photo[opcode_len] = {0x07, 0x00};
        constexpr uint8_t temperature_mode[opcode_len] = {0x08, 0x00};
        constexpr uint8_t acs_mode[opcode_len] = {0x09, 0x00};
        constexpr uint8_t change_simplified_acs[opcode_len] = {0x0A, 0x00};
        constexpr uint8_t camera_turn_on[opcode_len] = {0x0B, 0x00};
        constexpr uint8_t camera_turn_off[opcode_len] = {0x0C, 0x00};
        constexpr uint8_t request_imu_downlink_fragment[opcode_len] = {0x0D, 0x00}; // change
        constexpr uint8_t fault_mode[opcode_len] = {0xF1, 0xFF};
        constexpr uint8_t fault_check_mag_x[opcode_len] = {0xF2, 0xFF};
        constexpr uint8_t fault_check_mag_y[opcode_len] = {0xF3, 0xFF};
        constexpr uint8_t fault_check_mag_z[opcode_len] = {0xF4, 0xFF};
        constexpr uint8_t fault_check_gyro_x[opcode_len] = {0xF5, 0xFF};
        constexpr uint8_t fault_check_gyro_y[opcode_len] = {0xF6, 0xFF};
        constexpr uint8_t fault_check_gyro_z[opcode_len] = {0xF7, 0xFF};
        constexpr uint8_t fault_check_temp_c[opcode_len] = {0xF8, 0xFF};
        constexpr uint8_t fault_check_solar_current[opcode_len] = {0xF9, 0xFF};
        constexpr uint8_t fault_check_voltage[opcode_len] = {0xFA, 0xFF};

        constexpr uint8_t initialization[arg1_len] = {0x00, 0x00, 0x00, 0x00};
        constexpr uint8_t low_power[arg1_len] = {0x01, 0x00, 0x00, 0x00};
        constexpr uint8_t deployment[arg1_len] = {0x02, 0x00, 0x00, 0x00};
        constexpr uint8_t standby[arg1_len] = {0x03, 0x00, 0x00, 0x00};
        constexpr uint8_t safe[arg1_len] = {0x03, 0x00, 0x00, 0x00};
        constexpr uint8_t true_arg[arg1_len] = {0x01, 0x00, 0x00, 0x00};
        constexpr uint8_t false_arg[arg1_len] = {0x00, 0x00, 0x00, 0x00};
        constexpr uint8_t active[arg1_len] = {0x01, 0x00, 0x00, 0x00};
        constexpr uint8_t inactive[arg1_len] = {0x00, 0x00, 0x00, 0x00};
        constexpr uint8_t detumble[arg1_len] = {0x02, 0x00, 0x00, 0x00};
        constexpr uint8_t point[arg1_len] = {0x01, 0x00, 0x00, 0x00};
        constexpr uint8_t off[arg1_len] = {0x00, 0x00, 0x00, 0x00};
        constexpr uint8_t x[arg1_len] = {0x00, 0x00, 0x00, 0x00};
        constexpr uint8_t y[arg1_len] = {0x01, 0x00, 0x00, 0x00};
        constexpr uint8_t z[arg1_len] = {0x02, 0x00, 0x00, 0x00};

        constexpr uint8_t no_arg_2[arg2_len] = {0x00, 0x00, 0x00, 0x00};

        constexpr uint8_t mission_mode_low_power[command_len] = {
            mission_mode[0], mission_mode[1],
            low_power[0], low_power[1], low_power[2], low_power[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t mission_mode_deployment[command_len] = {
            mission_mode[0], mission_mode[1],
            deployment[0], deployment[1], deployment[2], deployment[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t mission_mode_standby[command_len] = {
            mission_mode[0], mission_mode[1],
            standby[0], standby[1], standby[2], standby[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t mission_mode_safe[command_len] = {
            mission_mode[0], mission_mode[1],
            safe[0], safe[1], safe[2], safe[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t mission_mode_initialization[command_len] = {
            mission_mode[0], mission_mode[1],
            initialization[0], initialization[1], initialization[2], initialization[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t burnwire_arm_true[command_len] = {
            burnwire_arm[0], burnwire_arm[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t burnwire_arm_false[command_len] = {
            burnwire_arm[0], burnwire_arm[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t burnwire_fire_true[command_len] = {
            burnwire_fire[0], burnwire_fire[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t burnwire_fire_false[command_len] = {
            burnwire_fire[0], burnwire_fire[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t camera_take_photo_true[command_len] = {
            camera_take_photo[0], camera_take_photo[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t camera_take_photo_false[command_len] = {
            camera_take_photo[0], camera_take_photo[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t temperature_mode_active[command_len] = {
            temperature_mode[0], temperature_mode[1],
            active[0], active[1], active[2], active[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t temperature_mode_inactive[command_len] = {
            temperature_mode[0], temperature_mode[1],
            inactive[0], inactive[1], inactive[2], inactive[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t acs_mode_detumble[command_len] = {
            acs_mode[0], acs_mode[1],
            detumble[0], detumble[1], detumble[2], detumble[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t acs_mode_point[command_len] = {
            acs_mode[0], acs_mode[1],
            point[0], point[1], point[2], point[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t acs_mode_off[command_len] = {
            acs_mode[0], acs_mode[1],
            off[0], off[1], off[2], off[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_mode_active[command_len] = {
            fault_mode[0], fault_mode[1],
            active[0], active[1], active[2], active[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_mode_inactive[command_len] = {
            fault_mode[0], fault_mode[1],
            inactive[0], inactive[1], inactive[2], inactive[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_mag_x_true[command_len] = {
            fault_check_mag_x[0], fault_check_mag_x[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_mag_x_false[command_len] = {
            fault_check_mag_x[0], fault_check_mag_x[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_mag_y_true[command_len] = {
            fault_check_mag_y[0], fault_check_mag_y[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_mag_y_false[command_len] = {
            fault_check_mag_y[0], fault_check_mag_y[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_mag_z_true[command_len] = {
            fault_check_mag_z[0], fault_check_mag_z[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_mag_z_false[command_len] = {
            fault_check_mag_z[0], fault_check_mag_z[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_gyro_x_true[command_len] = {
            fault_check_gyro_x[0], fault_check_gyro_x[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_gyro_x_false[command_len] = {
            fault_check_gyro_x[0], fault_check_gyro_x[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_gyro_y_true[command_len] = {
            fault_check_gyro_y[0], fault_check_gyro_y[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_gyro_y_false[command_len] = {
            fault_check_gyro_y[0], fault_check_gyro_y[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_gyro_z_true[command_len] = {
            fault_check_gyro_z[0], fault_check_gyro_z[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_gyro_z_false[command_len] = {
            fault_check_gyro_z[0], fault_check_gyro_z[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_temp_c_true[command_len] = {
            fault_check_temp_c[0], fault_check_temp_c[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_temp_c_false[command_len] = {
            fault_check_temp_c[0], fault_check_temp_c[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_solar_current_true[command_len] = {
            fault_check_solar_current[0], fault_check_solar_current[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_solar_current_false[command_len] = {
            fault_check_solar_current[0], fault_check_solar_current[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_voltage_true[command_len] = {
            fault_check_voltage[0], fault_check_voltage[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t fault_check_voltage_false[command_len] = {
            fault_check_voltage[0], fault_check_voltage[1],
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t change_simplified_acs_x[command_len] = {
            change_simplified_acs[0], change_simplified_acs[1],
            x[0], x[1], x[2], x[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t change_simplified_acs_y[command_len] = {
            change_simplified_acs[0], change_simplified_acs[1],
            y[0], y[1], y[2], y[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t change_simplified_acs_z[command_len] = {
            change_simplified_acs[0], change_simplified_acs[1],
            z[0], z[1], z[2], z[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t camera_turn_on_true[command_len] = {
            camera_turn_on[0], camera_turn_on[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr uint8_t camera_turn_off_true[command_len] = {
            camera_turn_off[0], camera_turn_off[1],
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr int known_commands[num_commands][command_len] = {
            {mission_mode_low_power[0], mission_mode_low_power[1], mission_mode_low_power[2], mission_mode_low_power[3], mission_mode_low_power[4], mission_mode_low_power[5], mission_mode_low_power[6], mission_mode_low_power[7], mission_mode_low_power[8], mission_mode_low_power[9]},
            {mission_mode_deployment[0], mission_mode_deployment[1], mission_mode_deployment[2], mission_mode_deployment[3], mission_mode_deployment[4], mission_mode_deployment[5], mission_mode_deployment[6], mission_mode_deployment[7], mission_mode_deployment[8], mission_mode_deployment[9]},
            {mission_mode_standby[0], mission_mode_standby[1], mission_mode_standby[2], mission_mode_standby[3], mission_mode_standby[4], mission_mode_standby[5], mission_mode_standby[6], mission_mode_standby[7], mission_mode_standby[8], mission_mode_standby[9]},
            {mission_mode_safe[0], mission_mode_safe[1], mission_mode_safe[2], mission_mode_safe[3], mission_mode_safe[4], mission_mode_safe[5], mission_mode_safe[6], mission_mode_safe[7], mission_mode_safe[8], mission_mode_safe[9]},
            {mission_mode_initialization[0], mission_mode_initialization[1], mission_mode_initialization[2], mission_mode_initialization[3], mission_mode_initialization[4], mission_mode_initialization[5], mission_mode_initialization[6], mission_mode_initialization[7], mission_mode_initialization[8], mission_mode_initialization[9]},
            {burnwire_arm_true[0], burnwire_arm_true[1], burnwire_arm_true[2], burnwire_arm_true[3], burnwire_arm_true[4], burnwire_arm_true[5], burnwire_arm_true[6], burnwire_arm_true[7], burnwire_arm_true[8], burnwire_arm_true[9]},
            {burnwire_arm_false[0], burnwire_arm_false[1], burnwire_arm_false[2], burnwire_arm_false[3], burnwire_arm_false[4], burnwire_arm_false[5], burnwire_arm_false[6], burnwire_arm_false[7], burnwire_arm_false[8], burnwire_arm_false[9]},
            {burnwire_fire_true[0], burnwire_fire_true[1], burnwire_fire_true[2], burnwire_fire_true[3], burnwire_fire_true[4], burnwire_fire_true[5], burnwire_fire_true[6], burnwire_fire_true[7], burnwire_fire_true[8], burnwire_fire_true[9]},
            {burnwire_fire_false[0], burnwire_fire_false[1], burnwire_fire_false[2], burnwire_fire_false[3], burnwire_fire_false[4], burnwire_fire_false[5], burnwire_fire_false[6], burnwire_fire_false[7], burnwire_fire_false[8], burnwire_fire_false[9]},
            {camera_take_photo_true[0], camera_take_photo_true[1], camera_take_photo_true[2], camera_take_photo_true[3], camera_take_photo_true[4], camera_take_photo_true[5], camera_take_photo_true[6], camera_take_photo_true[7], camera_take_photo_true[8], camera_take_photo_true[9]},
            {camera_take_photo_false[0], camera_take_photo_false[1], camera_take_photo_false[2], camera_take_photo_false[3], camera_take_photo_false[4], camera_take_photo_false[5], camera_take_photo_false[6], camera_take_photo_false[7], camera_take_photo_false[8], camera_take_photo_false[9]},
            {temperature_mode_active[0], temperature_mode_active[1], temperature_mode_active[2], temperature_mode_active[3], temperature_mode_active[4], temperature_mode_active[5], temperature_mode_active[6], temperature_mode_active[7], temperature_mode_active[8], temperature_mode_active[9]},
            {temperature_mode_inactive[0], temperature_mode_inactive[1], temperature_mode_inactive[2], temperature_mode_inactive[3], temperature_mode_inactive[4], temperature_mode_inactive[5], temperature_mode_inactive[6], temperature_mode_inactive[7], temperature_mode_inactive[8], temperature_mode_inactive[9]},
            {acs_mode_detumble[0], acs_mode_detumble[1], acs_mode_detumble[2], acs_mode_detumble[3], acs_mode_detumble[4], acs_mode_detumble[5], acs_mode_detumble[6], acs_mode_detumble[7], acs_mode_detumble[8], acs_mode_detumble[9]},
            {acs_mode_point[0], acs_mode_point[1], acs_mode_point[2], acs_mode_point[3], acs_mode_point[4], acs_mode_point[5], acs_mode_point[6], acs_mode_point[7], acs_mode_point[8], acs_mode_point[9]},
            {acs_mode_off[0], acs_mode_off[1], acs_mode_off[2], acs_mode_off[3], acs_mode_off[4], acs_mode_off[5], acs_mode_off[6], acs_mode_off[7], acs_mode_off[8], acs_mode_off[9]},
            {fault_mode_active[0], fault_mode_active[1], fault_mode_active[2], fault_mode_active[3], fault_mode_active[4], fault_mode_active[5], fault_mode_active[6], fault_mode_active[7], fault_mode_active[8], fault_mode_active[9]},
            {fault_mode_inactive[0], fault_mode_inactive[1], fault_mode_inactive[2], fault_mode_inactive[3], fault_mode_inactive[4], fault_mode_inactive[5], fault_mode_inactive[6], fault_mode_inactive[7], fault_mode_inactive[8], fault_mode_inactive[9]},
            {fault_check_mag_x_true[0], fault_check_mag_x_true[1], fault_check_mag_x_true[2], fault_check_mag_x_true[3], fault_check_mag_x_true[4], fault_check_mag_x_true[5], fault_check_mag_x_true[6], fault_check_mag_x_true[7], fault_check_mag_x_true[8], fault_check_mag_x_true[9]},
            {fault_check_mag_x_false[0], fault_check_mag_x_false[1], fault_check_mag_x_false[2], fault_check_mag_x_false[3], fault_check_mag_x_false[4], fault_check_mag_x_false[5], fault_check_mag_x_false[6], fault_check_mag_x_false[7], fault_check_mag_x_false[8], fault_check_mag_x_false[9]},
            {fault_check_mag_y_true[0], fault_check_mag_y_true[1], fault_check_mag_y_true[2], fault_check_mag_y_true[3], fault_check_mag_y_true[4], fault_check_mag_y_true[5], fault_check_mag_y_true[6], fault_check_mag_y_true[7], fault_check_mag_y_true[8], fault_check_mag_y_true[9]},
            {fault_check_mag_y_false[0], fault_check_mag_y_false[1], fault_check_mag_y_false[2], fault_check_mag_y_false[3], fault_check_mag_y_false[4], fault_check_mag_y_false[5], fault_check_mag_y_false[6], fault_check_mag_y_false[7], fault_check_mag_y_false[8], fault_check_mag_y_false[9]},
            {fault_check_mag_z_true[0], fault_check_mag_z_true[1], fault_check_mag_z_true[2], fault_check_mag_z_true[3], fault_check_mag_z_true[4], fault_check_mag_z_true[5], fault_check_mag_z_true[6], fault_check_mag_z_true[7], fault_check_mag_z_true[8], fault_check_mag_z_true[9]},
            {fault_check_mag_z_false[0], fault_check_mag_z_false[1], fault_check_mag_z_false[2], fault_check_mag_z_false[3], fault_check_mag_z_false[4], fault_check_mag_z_false[5], fault_check_mag_z_false[6], fault_check_mag_z_false[7], fault_check_mag_z_false[8], fault_check_mag_z_false[9]},
            {fault_check_gyro_x_true[0], fault_check_gyro_x_true[1], fault_check_gyro_x_true[2], fault_check_gyro_x_true[3], fault_check_gyro_x_true[4], fault_check_gyro_x_true[5], fault_check_gyro_x_true[6], fault_check_gyro_x_true[7], fault_check_gyro_x_true[8], fault_check_gyro_x_true[9]},
            {fault_check_gyro_x_false[0], fault_check_gyro_x_false[1], fault_check_gyro_x_false[2], fault_check_gyro_x_false[3], fault_check_gyro_x_false[4], fault_check_gyro_x_false[5], fault_check_gyro_x_false[6], fault_check_gyro_x_false[7], fault_check_gyro_x_false[8], fault_check_gyro_x_false[9]},
            {fault_check_gyro_y_true[0], fault_check_gyro_y_true[1], fault_check_gyro_y_true[2], fault_check_gyro_y_true[3], fault_check_gyro_y_true[4], fault_check_gyro_y_true[5], fault_check_gyro_y_true[6], fault_check_gyro_y_true[7], fault_check_gyro_y_true[8], fault_check_gyro_y_true[9]},
            {fault_check_gyro_y_false[0], fault_check_gyro_y_false[1], fault_check_gyro_y_false[2], fault_check_gyro_y_false[3], fault_check_gyro_y_false[4], fault_check_gyro_y_false[5], fault_check_gyro_y_false[6], fault_check_gyro_y_false[7], fault_check_gyro_y_false[8], fault_check_gyro_y_false[9]},
            {fault_check_gyro_z_true[0], fault_check_gyro_z_true[1], fault_check_gyro_z_true[2], fault_check_gyro_z_true[3], fault_check_gyro_z_true[4], fault_check_gyro_z_true[5], fault_check_gyro_z_true[6], fault_check_gyro_z_true[7], fault_check_gyro_z_true[8], fault_check_gyro_z_true[9]},
            {fault_check_gyro_z_false[0], fault_check_gyro_z_false[1], fault_check_gyro_z_false[2], fault_check_gyro_z_false[3], fault_check_gyro_z_false[4], fault_check_gyro_z_false[5], fault_check_gyro_z_false[6], fault_check_gyro_z_false[7], fault_check_gyro_z_false[8], fault_check_gyro_z_false[9]},
            {fault_check_temp_c_true[0], fault_check_temp_c_true[1], fault_check_temp_c_true[2], fault_check_temp_c_true[3], fault_check_temp_c_true[4], fault_check_temp_c_true[5], fault_check_temp_c_true[6], fault_check_temp_c_true[7], fault_check_temp_c_true[8], fault_check_temp_c_true[9]},
            {fault_check_temp_c_false[0], fault_check_temp_c_false[1], fault_check_temp_c_false[2], fault_check_temp_c_false[3], fault_check_temp_c_false[4], fault_check_temp_c_false[5], fault_check_temp_c_false[6], fault_check_temp_c_false[7], fault_check_temp_c_false[8], fault_check_temp_c_false[9]},
            {fault_check_solar_current_true[0], fault_check_solar_current_true[1], fault_check_solar_current_true[2], fault_check_solar_current_true[3], fault_check_solar_current_true[4], fault_check_solar_current_true[5], fault_check_solar_current_true[6], fault_check_solar_current_true[7], fault_check_solar_current_true[8], fault_check_solar_current_true[9]},
            {fault_check_solar_current_false[0], fault_check_solar_current_false[1], fault_check_solar_current_false[2], fault_check_solar_current_false[3], fault_check_solar_current_false[4], fault_check_solar_current_false[5], fault_check_solar_current_false[6], fault_check_solar_current_false[7], fault_check_solar_current_false[8], fault_check_solar_current_false[9]},
            {fault_check_voltage_true[0], fault_check_voltage_true[1], fault_check_voltage_true[2], fault_check_voltage_true[3], fault_check_voltage_true[4], fault_check_voltage_true[5], fault_check_voltage_true[6], fault_check_voltage_true[7], fault_check_voltage_true[8], fault_check_voltage_true[9]},
            {fault_check_voltage_false[0], fault_check_voltage_false[1], fault_check_voltage_false[2], fault_check_voltage_false[3], fault_check_voltage_false[4], fault_check_voltage_false[5], fault_check_voltage_false[6], fault_check_voltage_false[7], fault_check_voltage_false[8], fault_check_voltage_false[9]},
            {change_simplified_acs_x[0], change_simplified_acs_x[1], change_simplified_acs_x[2], change_simplified_acs_x[3], change_simplified_acs_x[4], change_simplified_acs_x[5], change_simplified_acs_x[6], change_simplified_acs_x[7], change_simplified_acs_x[8], change_simplified_acs_x[9]},
            {change_simplified_acs_y[0], change_simplified_acs_y[1], change_simplified_acs_y[2], change_simplified_acs_y[3], change_simplified_acs_y[4], change_simplified_acs_y[5], change_simplified_acs_y[6], change_simplified_acs_y[7], change_simplified_acs_y[8], change_simplified_acs_y[9]},
            {change_simplified_acs_z[0], change_simplified_acs_z[1], change_simplified_acs_z[2], change_simplified_acs_z[3], change_simplified_acs_z[4], change_simplified_acs_z[5], change_simplified_acs_z[6], change_simplified_acs_z[7], change_simplified_acs_z[8], change_simplified_acs_z[9]},
            {camera_turn_on_true[0], camera_turn_on_true[1], camera_turn_on_true[2], camera_turn_on_true[3], camera_turn_on_true[4], camera_turn_on_true[5], camera_turn_on_true[6], camera_turn_on_true[7], camera_turn_on_true[8], camera_turn_on_true[9]},
            {camera_turn_off_true[0], camera_turn_off_true[1], camera_turn_off_true[2], camera_turn_off_true[3], camera_turn_off_true[4], camera_turn_off_true[5], camera_turn_off_true[6], camera_turn_off_true[7], camera_turn_off_true[8], camera_turn_off_true[9]}};
    } // namespace rockblock
    namespace temperature {
        constexpr int pin = 39;
        constexpr float min_temp_c = -25;
        constexpr float max_temp_c = 65;
        constexpr float in_sun_val = 40;
    } // namespace temperature
    namespace current {
        constexpr int pin = 22;
        // TODO: finalize min/max
        constexpr float min_solar_current = 0;
        constexpr float max_solar_current = 300;
        // TODO: finalize in_sun_val
        constexpr int in_sun_val = 70;
        constexpr float voltage_ref = 3.3;
        constexpr int resolution = 1024;
        constexpr float load = 30;   // load resister value (kOhm)
        constexpr float shunt = 0.1; // shunt resistor value (Ohm)
    }                                // namespace current
    namespace acs {
        constexpr int xPWMpin = 10;
        constexpr int yPWMpin = 6;
        constexpr int zPWMpin = 30;

        constexpr int yout1 = 8;
        constexpr int yout2 = 7;

        constexpr int xout1 = 25;
        constexpr int xout2 = 24;

        constexpr int zout1 = 28;
        constexpr int zout2 = 29;

        constexpr int STBXYpin = 9;
        constexpr int STBZpin = 27;

        constexpr int xtorqorder = 0;
        constexpr int ytorqorder = 0;
        constexpr int ztorqorder = 0;

        //IMU Offset Coefficients

        //PWM fit coefficients 
        //ex: pwmX_ox_1: coefficient for first degree term in pwmX_ox
        constexpr float pwmX_ox_1 = 0.063563305433041769609836535437353; 
        constexpr float pwmX_ox_2 = 0.0000024534801053495480847952553427049; 
        constexpr float pwmX_ox_3 = -0.00000004048183650236316959656411693276; 
        
        constexpr float pwmX_oy_1 = - 0.016797609228196081626593993973984; 
        constexpr float pwmX_oy_2 = 0.0000080605967200896503065685530509121; 
        constexpr float pwmX_oy_3 = 0.000000012114204069568563619616825333992; 

        constexpr float pwmX_oz_1 = 0.0017958858966789052746815258387869;
        constexpr float pwmX_oz_2 = 0.000029717338935004530305191913774898; 
        constexpr float pwmX_oz_3 = 0.000000011774600680023870762861215393501; 
        ///////////////////////////////
        constexpr float pwmY_ox_1 = -0.029758606652877828135927984476439; 
        constexpr float pwmY_ox_2 = 0.0000036655843236024399974936228280287; 
        constexpr float pwmY_ox_3 =0.000000023498369011861784752980125408166; 
        
        constexpr float pwmY_oy_1 = 0.042815004635511522135260520371958; 
        constexpr float pwmY_oy_2 = 0.000015182122409661965729810693959134; 
        constexpr float pwmY_oy_3 = - 0.000000015456536073234678752023188506685; 

        constexpr float pwmY_oz_1 = - 0.064033443011555668533674179343507; 
        constexpr float pwmY_oz_2 = 0.000023898623196982134755635487910475; 
        constexpr float pwmY_oz_3 = 0.000000058017983244733767109801910464087; 
        /////////////////////////////
        constexpr float pwmZ_ox_1 = 0.049277911306007247949079186355448; 
        constexpr float pwmZ_ox_2 = 0.0000001823555642185477635673483036774; 
        constexpr float pwmZ_ox_3 = - 0.000000025703762479892763114309778196137; 
        
        constexpr float pwmZ_oy_1 = 0.015385071524851799337763047503813; 
        constexpr float pwmZ_oy_2 = 0.000018702657754292684140007621551582; 
        constexpr float pwmZ_oy_3 = - 0.0000000031989424202132932197155153338734; 

        constexpr float pwmZ_oz_1 = - 0.018823855991873226362054438709492; 
        constexpr float pwmZ_oz_2 = 0.000014901521761221710052694455761468; 
        constexpr float pwmZ_oz_3 = 0.000000014948952011168461910939879054225; 
        ///////////////////////////
        //Voltage fit coefficients 
        //ex: volX_ox_1, volX_ox_c --> coefficient and constant respectively for volX_ox
        constexpr float volX_ox_1 = 0.12375707365261180867327588256536; 
        constexpr float volX_ox_c = 0.48022029065903040357224129322547; 

        constexpr float volX_oy_1 = -0.54592452576565059280663991683131; 
        constexpr float volX_oy_c = 3.2928830082157324897878876506915; 

        constexpr float volX_oz_1 = 0.75132926778397910193914377979025; 
        constexpr float volX_oz_c = - 4.155582924692712228144403875119; 
        ///////////////////////////
        constexpr float volY_ox_1 = - 0.27909194272927493419007019171813; 
        constexpr float volY_ox_c = 2.1721861594629547235982948052162; 

        constexpr float volY_oy_1 = 0.16676521878709328946499592169343; 
        constexpr float volY_oy_c = 0.29958608109420818424701712888759; 

        constexpr float volY_oz_1 = - 0.14064119928172391235146520553687; 
        constexpr float volY_oz_c = -0.40930696301675956812384613674515; 
        ///////////////////////////
        constexpr float volZ_ox_1 = 0.15986532071754780144314062996388; 
        constexpr float volZ_ox_c = 0.32856565298629923393880935415171; 

        constexpr float volZ_oy_1 = 0.38192997034365841187064582326419; 
        constexpr float volZ_oy_c = - 0.60410587544336532985671245770959; 

        constexpr float volZ_oz_1 = -0.54213732000179207555231037777489; 
        constexpr float volZ_oz_c = 1.2769767440075267173197035866545; 
        ///////////////////////////
        //Temperature fit coefficients
        constexpr float temp_x_1 = -0.45645709660067923518766974666505; 
        constexpr float temp_x_2 = 0.0052611925857588160565514456834535; 
        constexpr float temp_x_3 = 0.000010529099180900968306677685515371; 
        constexpr float temp_x_c = 7.3908341297177386763905815314502; 

        constexpr float temp_y_1 = 0.32652431483093125041961002352764; 
        constexpr float temp_y_2 = - 0.0049475302634079910621411890758736; 
        constexpr float temp_y_3 = - 0.0000015243327341598422207456613175491;
        constexpr float temp_y_c = - 4.5352297389919185022222336556297;

        constexpr float temp_z_1 = 0.091402346089728289668663308020768; 
        constexpr float temp_z_2 = - 0.00061841171449482097276922898387852; 
        constexpr float temp_z_3 = 0.0000069097524971878865738986012778877; 
        constexpr float temp_z_c = - 1.7573124306246634684924856628641;

        //Hard Iron Offsets 
        constexpr float hardiron_x = -13.65; 
        constexpr float hardiron_y = 36.058; 
        constexpr float hardiron_z = 6.928; 

    } // namespace acs
    namespace battery {
        constexpr int voltage_value_pin = 32;
        constexpr int allow_measurement_pin = 36;
        constexpr int max_voltage = 5;
        constexpr int min_voltage = 3;
        constexpr float voltage_ref = 3.3;
        constexpr int resolution = 1023;
        constexpr int r1 = 4700;
        constexpr int r2 = 10000;
    } // namespace battery
    namespace fault {
        // fault 1
        constexpr uint8_t mag_x = 1 << 0;
        constexpr uint8_t mag_y = 1 << 1;
        constexpr uint8_t mag_z = 1 << 2;
        constexpr uint8_t gyro_x = 1 << 3;
        constexpr uint8_t gyro_y = 1 << 4;
        constexpr uint8_t gyro_z = 1 << 5;
        constexpr uint8_t acc_x = 1 << 6;
        constexpr uint8_t acc_y = 1 << 7;
        constexpr uint8_t imu_init = 1 << 6;

        // fault 2
        constexpr uint8_t temp_c = 1 << 0;
        constexpr uint8_t solar_current = 1 << 1;
        constexpr uint8_t voltage = 1 << 2;

        // fault 3
        constexpr uint8_t burn_wire = 1 << 0;
        constexpr uint8_t sd_card = 1 << 1;
        constexpr uint8_t camera_on_failed = 1 << 2;
        constexpr uint8_t light_val = 1 << 3;
    } // namespace fault
    namespace camera {
        constexpr int power_on_pin = 31;
        constexpr int content_length = 64;
        constexpr int bytes_allocated_serial_opcode = 2;
        constexpr int bytes_allocated_fragment = 4;
        constexpr int tx = 34;
        constexpr int rx = 33;
    } // namespace camera
    namespace button {
        // low when door is opened
        constexpr int button_pin = 37;
    } // namespace button
    namespace imu {
        constexpr float gravity = 9.80665;
        constexpr float two_g = 2 * gravity;
        constexpr float four_g = 4 * gravity;
        constexpr float eight_g = 8 * gravity;

        constexpr float min_mag_x = 0.0;
        constexpr float max_mag_x = 100;
        constexpr float min_mag_y = 0.0;
        constexpr float max_mag_y = 100;
        constexpr float min_mag_z = -100;
        constexpr float max_mag_z = 100;

        constexpr float min_gyro_x = 0.0;
        constexpr float max_gyro_x = 10;
        constexpr float min_gyro_y = 0.0;
        constexpr float max_gyro_y = 10;
        constexpr float min_gyro_z = 0.0;
        constexpr float max_gyro_z = 10;

        constexpr int bytes_allocated_fragment = 3;
        constexpr int bytes_allocated_serial_opcode = 2;

        constexpr int CSAG = 21;
        constexpr int CSM = 20;

        constexpr int max_gyro_imu_report_size = 66;
    } // namespace imu
    namespace timecontrol {
        // Environment-based initializations of the control loop time.
        // control_cycle_time is the value actually used for timing. The
        // other constants are just informational.
        constexpr unsigned int control_cycle_time_ms = 250;
        constexpr unsigned int control_cycle_time_us = control_cycle_time_ms * 1000;
        constexpr unsigned int control_cycle_time = control_cycle_time_us;

        // number being added is the time length of the previous function
        // battery monitor takes max 60 us, so button monitor will start 60us after
        static constexpr unsigned int acs_monitor_offset = 0;     // time starts at 0
        static constexpr unsigned int battery_monitor_offset = 0; // to be determined
        static constexpr unsigned int button_monitor_offset = 0;
        static constexpr unsigned int camera_report_monitor_offset = 0;
        static constexpr unsigned int command_monitor_offset = 0;
        static constexpr unsigned int current_monitor_offset = 0;
        static constexpr unsigned int fault_monitor_offset = 0;
        static constexpr unsigned int imu_monitor_offset = 0;
        static constexpr unsigned int imu_downlink_offset = 0;
        static constexpr unsigned int normal_report_monitor_offset = 0;
        static constexpr unsigned int imudownlink_report_monitor_offset = 0;
        static constexpr unsigned int photoresistor_monitor_offset = 0;
        static constexpr unsigned int temperature_monitor_offset = 0;

        static constexpr unsigned int acs_control_task_offset = 0;
        static constexpr unsigned int burnwire_control_task_offset = 0;
        static constexpr unsigned int camera_control_task_offset = 0;
        static constexpr unsigned int rockblock_control_task_offset = 0;
        static constexpr unsigned int rockblock_report_monitor_offset = 0;
        static constexpr unsigned int temperature_control_task_offset = 0;
        static constexpr unsigned int mission_manager_offset = 0;
    } // namespace timecontrol
};    // namespace constants

#endif
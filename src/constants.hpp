#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <cstddef>
#include <map>
#include <stdint.h>

namespace constants {
    namespace time {
        constexpr unsigned long one_second = 1000;
        constexpr unsigned long one_minute = 60 * one_second;
        constexpr unsigned long one_hour = 60 * one_minute;
        constexpr unsigned long one_day = 24 * one_hour;
        constexpr unsigned long half_second = one_second / 2;
    } // namespace time
    namespace sensor {
        constexpr int collect = 3;
        constexpr int repeats = 10;
    } // namespace sensor
    namespace photoresistor {
        constexpr int pin = 38;
        constexpr int light_val = 200;
    } // namespace photoresistor
    namespace burnwire {
        constexpr int first_pin = 14;
        constexpr int second_pin = 15;
        constexpr int camera_max_attempts = 50;
    } // namespace burnwire
    namespace rockblock {
        constexpr int normal_report_command_default_max = 15;

        constexpr int content_length = 68;

        constexpr int sleep_pin = 19;

        constexpr int min_sleep_period = 2 * time::one_minute;

        constexpr int baud = 19200;
        constexpr size_t buffer_size = 63;
        constexpr size_t packet_size = 70;
        constexpr size_t num_commas = 5;
        constexpr size_t opcode_len = 2;
        constexpr size_t arg1_len = 4;
        constexpr size_t arg2_len = 4;
        constexpr size_t command_len = opcode_len + arg1_len + arg2_len;
        constexpr size_t max_conseq_read = 3;

        constexpr uint8_t end_of_normal_downlink_flag1 = 254;
        constexpr uint8_t end_of_normal_downlink_flag2 = 255;
        constexpr uint8_t end_of_command_upload_flag1 = 0;
        constexpr uint8_t end_of_command_upload_flag2 = 250;
        namespace opcodes {
            // SFRField Commands
            constexpr uint16_t sfr_field_opcode_min = 0x1100;
            constexpr uint16_t sfr_field_opcode_max = 0x2899;

            // Deploy Command
            constexpr uint16_t sfr_field_opcode_deploy = 0x3333;

            // Arm Command
            constexpr uint16_t sfr_field_opcode_arm = 0x4444;

            // Fire Command
            constexpr uint16_t sfr_field_opcode_fire = 0x5555;

        } // namespace opcodes
    }     // namespace rockblock
    namespace temperature {
        constexpr int pin = 39;
        constexpr float in_sun_val = 30;
    } // namespace temperature
    namespace current {
        constexpr int pin = 22;
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
    } // namespace acs
    namespace battery {
        constexpr int voltage_value_pin = 32;
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
        constexpr uint8_t button = 1 << 4;
    } // namespace fault
    namespace camera {
        constexpr int power_on_pin = 31;
        constexpr int content_length = 64;
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

        constexpr int CSAG = 21;
        constexpr int CSM = 20;

        constexpr int max_gyro_imu_report_size = 66;
        constexpr int bootIMU_min_run_time = 20000;          // 20 seconds before transition to bootCamera.
        constexpr int after_door_opens_min_run_time = 10000; // 10 seconds after the door opens
    }                                                        // namespace imu

    namespace imu_downlink {
        // Note this is how much data correlates to 30 seconds
        constexpr int downlink_FIFO_byte_length = 462;
        constexpr uint8_t imu_report_endflag1 = 0xFE;
        constexpr uint8_t imu_report_endflag2 = 0x92;
    } // namespace imu_downlink
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
        static constexpr unsigned int eeprom_control_task_offset = 0;
        static constexpr unsigned int mission_manager_offset = 0;
    } // namespace timecontrol
    namespace eeprom {
        // EEPROM SFR memory offsets, determined by size of the prevoius SFR value type + size of a boolean (1 byte) + offset of the previous SFR value
        // The boolean indicates whether or not to restore the value on bootup
        static constexpr unsigned int stabilization_max_time_offset = 0;

        static constexpr unsigned int boot_max_time_offset = stabilization_max_time_offset + 5;

        static constexpr unsigned int simple_max_time_offset = boot_max_time_offset + 5;

        static constexpr unsigned int point_max_time_offset = simple_max_time_offset + 5;

        static constexpr unsigned int detumble_start_time_offset = point_max_time_offset + 5;
        static constexpr unsigned int detumble_max_time_offset = detumble_start_time_offset + 5;
        static constexpr unsigned int detumble_min_stable_gyro_z_offset = detumble_max_time_offset + 5;
        static constexpr unsigned int detumble_max_stable_gyro_x_offset = detumble_min_stable_gyro_z_offset + 2;
        static constexpr unsigned int detumble_max_stable_gyro_y_offset = detumble_max_stable_gyro_x_offset + 2;
        static constexpr unsigned int detumble_min_unstable_gyro_x_offset = detumble_max_stable_gyro_y_offset + 2;
        static constexpr unsigned int detumble_min_unstable_gyro_y_offset = detumble_min_unstable_gyro_x_offset + 2;

        static constexpr unsigned int aliveSignal_max_downlink_hard_faults_offset = detumble_min_unstable_gyro_y_offset + 2;
        static constexpr unsigned int aliveSignal_downlinked_offset = aliveSignal_max_downlink_hard_faults_offset + 3;
        static constexpr unsigned int aliveSignal_max_time_offset = aliveSignal_downlinked_offset + 2;
        static constexpr unsigned int aliveSignal_num_hard_faults_offset = aliveSignal_max_time_offset + 5;

        static constexpr unsigned int photoresistor_covered_offset = aliveSignal_num_hard_faults_offset + 3;

        static constexpr unsigned int mission_acs_transmit_cycle_time_offset = photoresistor_covered_offset + 2;
        static constexpr unsigned int mission_time_deployed_offset = mission_acs_transmit_cycle_time_offset + 5;
        static constexpr unsigned int mission_deployed_offset = mission_time_deployed_offset + 5;
        static constexpr unsigned int mission_already_deployed_offset = mission_deployed_offset + 2;
        static constexpr unsigned int mission_possible_uncovered_offset = mission_already_deployed_offset + 2;

        static constexpr unsigned int burnwire_attempts_offset = mission_possible_uncovered_offset + 2;
        static constexpr unsigned int burnwire_start_time_offset = burnwire_attempts_offset + 3;
        static constexpr unsigned int burnwire_burn_time_offset = burnwire_start_time_offset + 5;
        static constexpr unsigned int burnwire_armed_time_offset = burnwire_burn_time_offset + 5;
        static constexpr unsigned int burnwire_mode_offset = burnwire_armed_time_offset + 5;
        static constexpr unsigned int burnwire_attempts_limit_offset = burnwire_mode_offset + 3;
        static constexpr unsigned int burnwire_mandatory_attempts_limit_offset = burnwire_attempts_limit_offset + 3;
        static constexpr unsigned int burnwire_delay_time_offset = burnwire_mandatory_attempts_limit_offset + 3;

        static constexpr unsigned int camera_photo_taken_sd_failed_offset = burnwire_delay_time_offset + 5;
        static constexpr unsigned int camera_take_photo_offset = camera_photo_taken_sd_failed_offset + 2;
        static constexpr unsigned int camera_turn_on_offset = camera_take_photo_offset + 2;
        static constexpr unsigned int camera_turn_off_offset = camera_turn_on_offset + 2;
        static constexpr unsigned int camera_powered_offset = camera_turn_off_offset + 2;
        static constexpr unsigned int camera_start_progress_offset = camera_powered_offset + 2;
        static constexpr unsigned int camera_step_time_offset = camera_start_progress_offset + 2;
        static constexpr unsigned int camera_init_start_time_offset = camera_step_time_offset + 5;
        static constexpr unsigned int camera_init_timeout_offset = camera_init_start_time_offset + 5;
        static constexpr unsigned int camera_begin_delay_offset = camera_init_timeout_offset + 5;
        static constexpr unsigned int camera_resolution_set_delay_offset = camera_begin_delay_offset + 5;
        static constexpr unsigned int camera_resolution_get_delay_offset = camera_resolution_set_delay_offset + 5;
        static constexpr unsigned int camera_init_mode_offset = camera_resolution_get_delay_offset + 5;
        static constexpr unsigned int camera_mode_offset = camera_init_mode_offset + 3;
        static constexpr unsigned int camera_images_written_offset = camera_mode_offset + 3;
        static constexpr unsigned int camera_fragments_written_offset = camera_images_written_offset + 5;
        static constexpr unsigned int camera_set_res_offset = camera_fragments_written_offset + 5;
        static constexpr unsigned int camera_failed_times_offset = camera_set_res_offset + 5;
        static constexpr unsigned int camera_failed_limit_offset = camera_failed_times_offset + 3;
        static constexpr unsigned int camera_fragment_number_requested = camera_failed_limit_offset + 3;
        static constexpr unsigned int camera_serial_requested = camera_fragment_number_requested + 5;

        static constexpr unsigned int rockblock_ready_status_offset = camera_serial_requested + 2;
        static constexpr unsigned int rockblock_last_downlink_offset = rockblock_ready_status_offset + 2;
        static constexpr unsigned int rockblock_downlink_period_offset = rockblock_last_downlink_offset + 5;
        static constexpr unsigned int rockblock_waiting_message_offset = rockblock_downlink_period_offset + 5;
        static constexpr unsigned int rockblock_max_commands_count_offset = rockblock_waiting_message_offset + 2;
        static constexpr unsigned int rockblock_imu_max_fragments_offset = rockblock_max_commands_count_offset + 2;
        static constexpr unsigned int rockblock_imudownlink_start_time_offset = rockblock_imu_max_fragments_offset + 3;
        static constexpr unsigned int rockblock_imudownlink_remain_time_offset = rockblock_imudownlink_start_time_offset + 5;
        static constexpr unsigned int rockblock_imu_first_start_offset = rockblock_imudownlink_remain_time_offset + 5;
        static constexpr unsigned int rockblock_imu_downlink_on_offset = rockblock_imu_first_start_offset + 2;
        static constexpr unsigned int rockblock_flush_status_offset = rockblock_imu_downlink_on_offset + 2;
        static constexpr unsigned int rockblock_waiting_command_offset = rockblock_flush_status_offset + 2;
        static constexpr unsigned int rockblock_conseq_reads_offset = rockblock_waiting_command_offset + 2;
        static constexpr unsigned int rockblock_timeout_offset = rockblock_conseq_reads_offset + 5;
        static constexpr unsigned int rockblock_start_time_offset = rockblock_timeout_offset + 5;
        static constexpr unsigned int rockblock_start_time_check_signal_offset = rockblock_start_time_offset + 5;
        static constexpr unsigned int rockblock_max_check_signal_time_offset = rockblock_start_time_check_signal_offset + 5;
        static constexpr unsigned int rockblock_sleep_mode_offset = rockblock_max_check_signal_time_offset + 5;
        static constexpr unsigned int rockblock_downlink_report_type_offset = rockblock_sleep_mode_offset + 2;
        static constexpr unsigned int rockblock_mode_offset = rockblock_downlink_report_type_offset + 3;

        static constexpr unsigned int imu_mode_offset = rockblock_mode_offset + 3;
        static constexpr unsigned int imu_init_mode_offset = imu_mode_offset + 3;
        static constexpr unsigned int imu_max_fragments_offset = imu_init_mode_offset + 3;
        static constexpr unsigned int imu_sample_gyro_offset = imu_max_fragments_offset + 5;
        static constexpr unsigned int imu_turn_on_offset = imu_sample_gyro_offset + 2;
        static constexpr unsigned int imu_turn_off_offset = imu_turn_on_offset + 2;
        static constexpr unsigned int imu_powered_offset = imu_turn_off_offset + 2;
        static constexpr unsigned int imu_failed_times_offset = imu_powered_offset + 2;
        static constexpr unsigned int imu_failed_limit_offset = imu_failed_times_offset + 3;

        static constexpr unsigned int temperature_in_sun_offset = imu_failed_limit_offset + 3;

        static constexpr unsigned int current_in_sun_offset = temperature_in_sun_offset + 2;

        static constexpr unsigned int acs_max_no_communication_offset = current_in_sun_offset + 2;
        static constexpr unsigned int acs_on_time_offset = acs_max_no_communication_offset + 5;
        static constexpr unsigned int acs_off_offset = acs_on_time_offset + 5;
        static constexpr unsigned int acs_mag_offset = acs_off_offset + 2;
        static constexpr unsigned int acs_detumble_timeout_offset = acs_mag_offset + 3;

        static constexpr unsigned int battery_acceptable_battery_offset = acs_detumble_timeout_offset + 5;
        static constexpr unsigned int battery_min_battery_offset = battery_acceptable_battery_offset + 5;
        static constexpr unsigned int button_pressed_offset = battery_min_battery_offset + 5;

        static constexpr unsigned int eeprom_boot_count_offset = button_pressed_offset + 2;
        static constexpr unsigned int eeprom_wait_time_write_step_time_offset = eeprom_boot_count_offset + 2;
        static constexpr unsigned int eeprom_alloted_time_offset = eeprom_wait_time_write_step_time_offset + 5;
        static constexpr unsigned int eeprom_alloted_time_passed_offset = eeprom_alloted_time_offset + 5;
        static constexpr unsigned int eeprom_sfr_write_step_time_offset = eeprom_alloted_time_passed_offset + 2;
        static constexpr unsigned int eeprom_sfr_address_age_offset = eeprom_sfr_write_step_time_offset + 5;
        static constexpr unsigned int eeprom_storage_full_offset = eeprom_sfr_address_age_offset + 5;

        static constexpr unsigned int full_offset = eeprom_storage_full_offset + 2;
    } // namespace eeprom
};    // namespace constants

#endif
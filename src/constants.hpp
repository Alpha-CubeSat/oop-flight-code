#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <cstddef>
#include <stdint.h>

namespace constants {
    namespace pins {
        // TODO confirm initial pin states are correct FS-159
        // @ Josh do we need to change this?
        std::map<int, int> pinMap = {
            {constants::photoresistor::pin, LOW},
            {constants::burnwire::first_pin, LOW},
            {constants::burnwire::second_pin, LOW},
            {constants::rockblock::sleep_pin, LOW},
            {constants::temperature::pin, LOW},
            {constants::current::pin, LOW},
            {constants::acs::xPWMpin, LOW},
            {constants::acs::yPWMpin, LOW},
            {constants::acs::zPWMpin, LOW},
            {constants::acs::yout1, LOW},
            {constants::acs::yout2, LOW},
            {constants::acs::xout1, LOW},
            {constants::acs::xout2, LOW},
            {constants::acs::zout1, LOW},
            {constants::acs::zout2, LOW},
            {constants::acs::STBXYpin, LOW},
            {constants::acs::STBZpin, LOW},
            {constants::battery::voltage_value_pin, LOW},
            {constants::battery::allow_measurement_pin, HIGH},
            {constants::camera::power_on_pin, LOW},
            {constants::camera::rx, LOW},
            {constants::camera::tx, LOW},
            {constants::button::button_pin, LOW}};
    } // namespace pins
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
        constexpr int content_length = 68;

        constexpr int sleep_pin = 19;

        constexpr int min_sleep_period = 2 * time::one_minute;

        constexpr int min_downlink_period = time::one_second;
        constexpr int max_downlink_period = 2 * time::one_day;

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
        constexpr uint8_t end_of_command_upload_flag1 = 0;
        constexpr uint8_t end_of_command_upload_flag2 = 250;
        namespace opcodes {
            // SFRField Commands
            constexpr uint16_t sfr_field_opcode_min = 0x1100;
            constexpr uint16_t sfr_field_opcode_max = 0x2899;

            // Fire Command
            constexpr uint16_t sfr_field_opcode_fire = 0x4444;

            // Arm Command
            constexpr uint16_t sfr_field_opcode_arm = 0x5555;

        } // namespace opcodes
        // namespace rockblock

        namespace args {
            // First Argument Payload 4-Bytes
            // Mission Mode Op Arg Values
            constexpr uint32_t BOOT = 0;
            constexpr uint32_t ALIVE_SIGNAL = 1;
            constexpr uint32_t LOW_POWER_ALIVE_SIGNAL = 2;
            constexpr uint32_t DETUMBLE_SPIN = 3;
            constexpr uint32_t LOW_POWER_DETUMBLE_SPIN = 4;
            constexpr uint32_t NORMAL = 5;
            constexpr uint32_t TRANSMIT = 6;
            constexpr uint32_t LOW_POWER = 7;
            constexpr uint32_t NORMAL_DEPLOYMENT = 8;
            constexpr uint32_t TRANSMIT_DEPLOYMENT = 9;
            constexpr uint32_t LOW_POWER_DEPLOYMENT = 10;
            constexpr uint32_t NORMAL_IN_SUN = 11;
            constexpr uint32_t TRANSMIT_IN_SUN = 12;
            constexpr uint32_t LOW_POWER_IN_SUN = 13;
            constexpr uint32_t VOLT_FAIL_IN_SUN = 14;
            constexpr uint32_t BOOT_CAMERA = 15;
            constexpr uint32_t MANDATORY_BURNS = 16;
            constexpr uint32_t REGULAR_BURNS = 17;
            constexpr uint32_t PHOTO = 18;

            constexpr uint32_t MISSION_MODE_MIN = BOOT;
            constexpr uint32_t MISSION_MODE_MAX = PHOTO;

            constexpr uint32_t true_arg = 1;
            constexpr uint32_t false_arg = 0;
            constexpr uint32_t active = 1;
            constexpr uint32_t inactive = 0;
            constexpr uint32_t full = 2;
            constexpr uint32_t simple = 1;
            constexpr uint32_t off = 0;
            constexpr uint32_t x = 0;
            constexpr uint32_t y = 1;
            constexpr uint32_t z = 2;
            constexpr uint32_t no_arg_2 = 0;
        } // namespace args
    }     // namespace rockblock
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
        constexpr uint8_t button = 1 << 4;
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
        static constexpr unsigned int eeprom_control_task_offset = 0;
        static constexpr unsigned int mission_manager_offset = 0;
    } // namespace timecontrol
    namespace eeprom {
        // EEPROM SFR memory offsets, determined by size of the prevoius SFR value type + size of a boolean + offset of the previous SFR value
        // The boolean indicates whether or not to restore the value on bootup
        static constexpr unsigned int stabilization_max_time_offset = 0;

        static constexpr unsigned int boot_max_time_offset = stabilization_max_time_offset + 6;

        static constexpr unsigned int simple_max_time_offset = boot_max_time_offset + 6;

        static constexpr unsigned int point_max_time_offset = simple_max_time_offset + 6;

        static constexpr unsigned int detumble_start_time_offset = point_max_time_offset + 6;
        static constexpr unsigned int detumble_max_time_offset = detumble_start_time_offset + 6;
        static constexpr unsigned int detumble_num_imu_retries_offset = detumble_max_time_offset + 6;
        static constexpr unsigned int detumble_max_imu_retries_offset = detumble_num_imu_retries_offset + 4;
        static constexpr unsigned int detumble_min_stable_gyro_z_offset = detumble_max_imu_retries_offset + 4;
        static constexpr unsigned int detumble_max_stable_gyro_x_offset = detumble_min_stable_gyro_z_offset + 3;
        static constexpr unsigned int detumble_max_stable_gyro_y_offset = detumble_max_stable_gyro_x_offset + 3;
        static constexpr unsigned int detumble_min_unstable_gyro_x_offset = detumble_max_stable_gyro_y_offset + 3;
        static constexpr unsigned int detumble_min_unstable_gyro_y_offset = detumble_min_unstable_gyro_x_offset + 3;

        static constexpr unsigned int aliveSignal_max_downlink_hard_faults_offset = detumble_min_unstable_gyro_y_offset + 3;
        static constexpr unsigned int aliveSignal_downlinked_offset = aliveSignal_max_downlink_hard_faults_offset + 4;
        static constexpr unsigned int aliveSignal_max_time_offset = aliveSignal_downlinked_offset + 4;
        static constexpr unsigned int aliveSignal_num_hard_faults_offset = aliveSignal_max_time_offset + 6;

        static constexpr unsigned int photoresistor_covered_offset = aliveSignal_num_hard_faults_offset + 4;

        static constexpr unsigned int mission_acs_transmit_cycle_time_offset = photoresistor_covered_offset + 4;
        static constexpr unsigned int mission_time_deployed_offset = mission_acs_transmit_cycle_time_offset + 6;
        static constexpr unsigned int mission_deployed_offset = mission_time_deployed_offset + 6;
        static constexpr unsigned int mission_already_deployed_offset = mission_deployed_offset + 4;

        static constexpr unsigned int burnwire_attempts_offset = mission_already_deployed_offset + 4;
        static constexpr unsigned int burnwire_camera_attempts_offset = burnwire_attempts_offset + 4;
        static constexpr unsigned int burnwire_start_time_offset = burnwire_camera_attempts_offset + 6;
        static constexpr unsigned int burnwire_burn_time_offset = burnwire_start_time_offset + 6;
        static constexpr unsigned int burnwire_armed_time_offset = burnwire_burn_time_offset + 6;
        static constexpr unsigned int burnwire_mode_offset = burnwire_armed_time_offset + 6;
        static constexpr unsigned int burnwire_attempts_limit_offset = burnwire_mode_offset + 4;

        static constexpr unsigned int camera_photo_taken_sd_failed_offset = burnwire_attempts_limit_offset + 4;
        static constexpr unsigned int camera_take_photo_offset = camera_photo_taken_sd_failed_offset + 4;
        static constexpr unsigned int camera_turn_on_offset = camera_take_photo_offset + 4;
        static constexpr unsigned int camera_turn_off_offset = camera_turn_on_offset + 4;
        static constexpr unsigned int camera_powered_offset = camera_turn_off_offset + 4;
        static constexpr unsigned int camera_start_progress_offset = camera_powered_offset + 4;
        static constexpr unsigned int camera_step_time_offset = camera_start_progress_offset + 3;
        static constexpr unsigned int camera_init_start_time_offset = camera_step_time_offset + 6;
        static constexpr unsigned int camera_init_timeout_offset = camera_init_start_time_offset + 6;
        static constexpr unsigned int camera_begin_delay_offset = camera_init_timeout_offset + 6;
        static constexpr unsigned int camera_resolution_set_delay_offset = camera_begin_delay_offset + 6;
        static constexpr unsigned int camera_resolution_get_delay_offset = camera_resolution_set_delay_offset + 6;
        static constexpr unsigned int camera_init_mode_offset = camera_resolution_get_delay_offset + 6;
        static constexpr unsigned int camera_mode_offset = camera_init_mode_offset + 4;
        static constexpr unsigned int camera_images_written_offset = camera_mode_offset + 4;
        static constexpr unsigned int camera_fragments_written_offset = camera_images_written_offset + 6;
        static constexpr unsigned int camera_set_res_offset = camera_fragments_written_offset + 6;
        static constexpr unsigned int camera_failed_times_offset = camera_set_res_offset + 6;
        static constexpr unsigned int camera_failed_limit_offset = camera_failed_times_offset + 4;

        static constexpr unsigned int rockblock_ready_status_offset = camera_failed_limit_offset + 4;
        static constexpr unsigned int rockblock_last_downlink_offset = rockblock_ready_status_offset + 4;
        static constexpr unsigned int rockblock_downlink_period_offset = rockblock_last_downlink_offset + 6;
        static constexpr unsigned int rockblock_waiting_message_offset = rockblock_downlink_period_offset + 6;
        static constexpr unsigned int rockblock_max_commands_count_offset = rockblock_waiting_message_offset + 4;
        static constexpr unsigned int rockblock_imu_max_fragments_offset = rockblock_max_commands_count_offset + 3;
        static constexpr unsigned int rockblock_imudownlink_start_time_offset = rockblock_imu_max_fragments_offset + 4;
        static constexpr unsigned int rockblock_imudownlink_remain_time_offset = rockblock_imudownlink_start_time_offset + 6;
        static constexpr unsigned int rockblock_imu_first_start_offset = rockblock_imudownlink_remain_time_offset + 6;
        static constexpr unsigned int rockblock_imu_downlink_on_offset = rockblock_imu_first_start_offset + 4;
        static constexpr unsigned int rockblock_flush_status_offset = rockblock_imu_downlink_on_offset + 4;
        static constexpr unsigned int rockblock_waiting_command_offset = rockblock_flush_status_offset + 4;
        static constexpr unsigned int rockblock_conseq_reads_offset = rockblock_waiting_command_offset + 4;
        static constexpr unsigned int rockblock_timeout_offset = rockblock_conseq_reads_offset + 6;
        static constexpr unsigned int rockblock_start_time_offset = rockblock_timeout_offset + 6;
        static constexpr unsigned int rockblock_start_time_check_signal_offset = rockblock_start_time_offset + 6;
        static constexpr unsigned int rockblock_max_check_signal_time_offset = rockblock_start_time_check_signal_offset + 6;
        static constexpr unsigned int rockblock_sleep_mode_offset = rockblock_max_check_signal_time_offset + 6;
        static constexpr unsigned int rockblock_downlink_report_type_offset = rockblock_sleep_mode_offset + 4;
        static constexpr unsigned int rockblock_mode_offset = rockblock_downlink_report_type_offset + 4;

        static constexpr unsigned int imu_mode_offset = rockblock_mode_offset + 4;
        static constexpr unsigned int imu_successful_init_offset = imu_mode_offset + 4;
        static constexpr unsigned int imu_max_fragments_offset = imu_successful_init_offset + 4;
        static constexpr unsigned int imu_sample_gyro_offset = imu_max_fragments_offset + 6;
        static constexpr unsigned int imu_turn_on_offset = imu_sample_gyro_offset + 4;
        static constexpr unsigned int imu_turn_off_offset = imu_turn_on_offset + 4;
        static constexpr unsigned int imu_powered_offset = imu_turn_off_offset + 4;

        static constexpr unsigned int temperature_in_sun_offset = imu_powered_offset + 4;

        static constexpr unsigned int current_in_sun_offset = temperature_in_sun_offset + 4;

        static constexpr unsigned int acs_max_no_communication_offset = current_in_sun_offset + 4;
        static constexpr unsigned int acs_on_time_offset = acs_max_no_communication_offset + 6;
        static constexpr unsigned int acs_off_offset = acs_on_time_offset + 6;
        static constexpr unsigned int acs_mag_offset = acs_off_offset + 4;

        static constexpr unsigned int battery_acceptable_battery_offset = acs_mag_offset + 4;
        static constexpr unsigned int battery_min_battery_offset = battery_acceptable_battery_offset + 6;

        static constexpr unsigned int button_pressed_offset = battery_min_battery_offset + 6;

        static constexpr unsigned int eeprom_wait_time_last_write_time_offset = button_pressed_offset + 4;
        static constexpr unsigned int eeprom_wait_time_write_step_time_offset = eeprom_wait_time_last_write_time_offset + 6;
        static constexpr unsigned int eeprom_alloted_time_offset = eeprom_wait_time_write_step_time_offset + 6;
        static constexpr unsigned int eeprom_alloted_time_passed_offset = eeprom_alloted_time_offset + 6;
        static constexpr unsigned int eeprom_sfr_last_write_time_offset = eeprom_alloted_time_passed_offset + 4;
        static constexpr unsigned int eeprom_sfr_write_step_time_offset = eeprom_sfr_last_write_time_offset + 6;
        static constexpr unsigned int eeprom_sfr_address_offset = eeprom_sfr_write_step_time_offset + 6;
        static constexpr unsigned int eeprom_sfr_address_age_offset = eeprom_sfr_address_offset + 4;

        static constexpr unsigned int full_offset = eeprom_sfr_address_age_offset + 4;
    } // namespace eeprom
};    // namespace constants

#endif
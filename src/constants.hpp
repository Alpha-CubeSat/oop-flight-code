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
    } // namespace time
    namespace sensor {
        constexpr int collect = 3;
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
        constexpr size_t checksum_len = 4;
        constexpr size_t command_len = opcode_len + arg1_len + arg2_len;
        constexpr size_t max_conseq_read = 3;

        constexpr int num_initial_downlinks = 2;
        constexpr uint8_t start_of_normal_downlink_flag = 99;
        constexpr uint8_t end_of_normal_downlink_flag1 = 254;
        constexpr uint8_t end_of_normal_downlink_flag2 = 255;
        constexpr uint8_t end_of_command_upload_flag1 = 0;
        constexpr uint8_t end_of_command_upload_flag2 = 250;
        namespace opcodes {
            constexpr uint16_t mission_mode = 0; // -> StateCommand //? Should this be Phase_mode?

            // Burnwire Commands
            constexpr uint16_t burnwire_arm = 1; // -> BurnwireCommand
            constexpr uint16_t burnwire_fire = 2;
            constexpr uint16_t burnwire_time = 3;
            constexpr uint16_t burnwire_timeout = 4;

            constexpr uint16_t rockblock_downlink_period[opcode_len] = {0x05, 0x00};

            // Camera Commands
            constexpr uint16_t request_image_fragment = 6;
            constexpr uint16_t camera_take_photo = 7;
            constexpr uint16_t camera_turn_on = 11;
            constexpr uint16_t camera_turn_off = 12;

            // SFRField Commands
            constexpr uint16_t sfr_field_opcode_min = 11;
            constexpr uint16_t sfr_field_opcode_max = 28;

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
};    // namespace constants

#endif
#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <cstddef>
#include <stdint.h>
#include <vector>

namespace constants {
    namespace time {
        constexpr unsigned long one_second = 1000;
        constexpr unsigned long one_minute = 60 * one_second;
        constexpr unsigned long one_hour = 60 * one_minute;
        constexpr unsigned long one_day = 24 * one_hour;
        constexpr unsigned long half_second = one_second / 2;
        constexpr float one_revolution = 90 * one_minute;

        // control_cycle_time is the value actually used for timing. The
        constexpr unsigned int control_cycle_time_ms = 100;
    } // namespace time
    namespace sensor {
        constexpr int collect = 3;
        constexpr int repeats = 10;
    } // namespace sensor
    namespace photoresistor {
        constexpr int pin = 38;
        constexpr int light_val = 200;
        constexpr int min_light_val = 0;
        constexpr int max_light_val = 1023;
    } // namespace photoresistor
    namespace burnwire {
        constexpr int first_pin = 14;
        constexpr int second_pin = 15;
    } // namespace burnwire
    namespace rockblock {
        constexpr int normal_report_command_max = 9;
        constexpr int mission_mode_hist_length = 16;
        constexpr int content_length = 68;
        constexpr int sleep_pin = 19;
        constexpr int min_sleep_period = 2 * time::one_minute;
        constexpr int max_check_signal_time = time::one_minute;
        constexpr int max_same_mode = (10 * time::one_minute) / time::control_cycle_time_ms;

        constexpr int baud = 19200;
        constexpr size_t buffer_size = 63;
        constexpr size_t num_commas = 5;
        constexpr size_t opcode_len = 2;
        constexpr size_t arg1_len = 4;
        constexpr size_t arg2_len = 4;
        constexpr size_t command_len = opcode_len + arg1_len + arg2_len;
        constexpr size_t max_conseq_read = 3;

        constexpr uint8_t normal_report_flag = 99;
        constexpr uint8_t imu_report_flag = 24;
        constexpr uint8_t camera_report_flag = 42;

        constexpr uint8_t end_of_command_upload_flag1 = 250;
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

            // Fault Suppress Commands
            constexpr uint16_t fault_opcode_min = 0x6000;
            constexpr uint16_t fault_opcode_max = 0x6100;

            // EEPROM Reset Command
            constexpr uint16_t eeprom_reset_opcode = 0x7777;

        } // namespace opcodes
    }     // namespace rockblock
    namespace temperature {
        constexpr int pin = 39;
        constexpr int in_sun_val = 30;
        constexpr int min_temp_c = -100;
        constexpr int max_temp_c = 200;
    } // namespace temperature
    namespace current {
        constexpr int pin = 22;
        constexpr int in_sun_val = 70;
        constexpr float voltage_ref = 3.3;
        constexpr int resolution = 1024;
        constexpr int load = 30;     // load resister value (kOhm)
        constexpr float shunt = 0.1; // shunt resistor value (Ohm)
    }                                // namespace current
    namespace masks {
        constexpr uint32_t uint32_byte1_mask = 0b11111111000000000000000000000000;
        constexpr uint32_t uint32_byte2_mask = 0b00000000111111110000000000000000;
        constexpr uint32_t uint32_byte3_mask = 0b00000000000000001111111100000000;
        constexpr uint32_t uint32_byte4_mask = 0b00000000000000000000000011111111;

    } // namespace masks
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

        // IMU Offset Coefficients

        // PWM fit coefficients
        // ex: pwmX_ox_1: coefficient for first degree term in pwmX_ox
        constexpr float pwmX_ox_1 = 0.063563305433041769609836535437353;
        constexpr float pwmX_ox_2 = 0.0000024534801053495480847952553427049;
        constexpr float pwmX_ox_3 = -0.00000004048183650236316959656411693276;

        constexpr float pwmX_oy_1 = -0.016797609228196081626593993973984;
        constexpr float pwmX_oy_2 = 0.0000080605967200896503065685530509121;
        constexpr float pwmX_oy_3 = 0.000000012114204069568563619616825333992;

        constexpr float pwmX_oz_1 = 0.0017958858966789052746815258387869;
        constexpr float pwmX_oz_2 = 0.000029717338935004530305191913774898;
        constexpr float pwmX_oz_3 = 0.000000011774600680023870762861215393501;
        ///////////////////////////////
        constexpr float pwmY_ox_1 = -0.029758606652877828135927984476439;
        constexpr float pwmY_ox_2 = 0.0000036655843236024399974936228280287;
        constexpr float pwmY_ox_3 = 0.000000023498369011861784752980125408166;

        constexpr float pwmY_oy_1 = 0.042815004635511522135260520371958;
        constexpr float pwmY_oy_2 = 0.000015182122409661965729810693959134;
        constexpr float pwmY_oy_3 = -0.000000015456536073234678752023188506685;

        constexpr float pwmY_oz_1 = -0.064033443011555668533674179343507;
        constexpr float pwmY_oz_2 = 0.000023898623196982134755635487910475;
        constexpr float pwmY_oz_3 = 0.000000058017983244733767109801910464087;
        /////////////////////////////
        constexpr float pwmZ_ox_1 = 0.049277911306007247949079186355448;
        constexpr float pwmZ_ox_2 = 0.0000001823555642185477635673483036774;
        constexpr float pwmZ_ox_3 = -0.000000025703762479892763114309778196137;

        constexpr float pwmZ_oy_1 = 0.015385071524851799337763047503813;
        constexpr float pwmZ_oy_2 = 0.000018702657754292684140007621551582;
        constexpr float pwmZ_oy_3 = -0.0000000031989424202132932197155153338734;

        constexpr float pwmZ_oz_1 = -0.018823855991873226362054438709492;
        constexpr float pwmZ_oz_2 = 0.000014901521761221710052694455761468;
        constexpr float pwmZ_oz_3 = 0.000000014948952011168461910939879054225;
        ///////////////////////////
        // Voltage fit coefficients
        // ex: volX_ox_1, volX_ox_c --> coefficient and constant respectively for volX_ox
        constexpr float volX_ox_1 = 0.12375707365261180867327588256536;
        constexpr float volX_ox_c = 0.48022029065903040357224129322547;

        constexpr float volX_oy_1 = -0.54592452576565059280663991683131;
        constexpr float volX_oy_c = 3.2928830082157324897878876506915;

        constexpr float volX_oz_1 = 0.75132926778397910193914377979025;
        constexpr float volX_oz_c = -4.155582924692712228144403875119;
        ///////////////////////////
        constexpr float volY_ox_1 = -0.27909194272927493419007019171813;
        constexpr float volY_ox_c = 2.1721861594629547235982948052162;

        constexpr float volY_oy_1 = 0.16676521878709328946499592169343;
        constexpr float volY_oy_c = 0.29958608109420818424701712888759;

        constexpr float volY_oz_1 = -0.14064119928172391235146520553687;
        constexpr float volY_oz_c = -0.40930696301675956812384613674515;
        ///////////////////////////
        constexpr float volZ_ox_1 = 0.15986532071754780144314062996388;
        constexpr float volZ_ox_c = 0.32856565298629923393880935415171;

        constexpr float volZ_oy_1 = 0.38192997034365841187064582326419;
        constexpr float volZ_oy_c = -0.60410587544336532985671245770959;

        constexpr float volZ_oz_1 = -0.54213732000179207555231037777489;
        constexpr float volZ_oz_c = 1.2769767440075267173197035866545;
        ///////////////////////////
        // Temperature fit coefficients
        constexpr float temp_x_1 = -0.45645709660067923518766974666505;
        constexpr float temp_x_2 = 0.0052611925857588160565514456834535;
        constexpr float temp_x_3 = 0.000010529099180900968306677685515371;
        constexpr float temp_x_c = 7.3908341297177386763905815314502;

        constexpr float temp_y_1 = 0.32652431483093125041961002352764;
        constexpr float temp_y_2 = -0.0049475302634079910621411890758736;
        constexpr float temp_y_3 = -0.0000015243327341598422207456613175491;
        constexpr float temp_y_c = -4.5352297389919185022222336556297;

        constexpr float temp_z_1 = 0.091402346089728289668663308020768;
        constexpr float temp_z_2 = -0.00061841171449482097276922898387852;
        constexpr float temp_z_3 = 0.0000069097524971878865738986012778877;
        constexpr float temp_z_c = -1.7573124306246634684924856628641;

        // Hard Iron Offsets
        constexpr float hardiron_x = -13.65;
        constexpr float hardiron_y = 36.058;
        constexpr float hardiron_z = 6.928;

        // Starshot
        constexpr float step_size_input = 0.20;
        constexpr float A_input = 4.0E-5;
        constexpr float i_max_input = 0.25;
        constexpr float k_input = 13.5;
        constexpr float n_input = 500.0;
        constexpr float Id_values[1] = {0.0021};
        constexpr float Kd_values[1] = {0.0007935279615795299};
        constexpr float Kp_values[1] = {5.2506307629097953E-10};
        constexpr float c_values[1] = {0.004};

    } // namespace acs
    namespace battery {
        constexpr int voltage_value_pin = 32;
        constexpr float voltage_ref = 3.3;
        constexpr int resolution = 1023;
        constexpr int r1 = 4700;
        constexpr int r2 = 10000;
        constexpr int min_voltage = 0;
        constexpr int max_voltage = 6;
        constexpr int sfr_resolution = 100;
    } // namespace battery
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
        constexpr float min_mag = -150;
        constexpr float max_mag = 150;

        constexpr float min_gyro = -5;
        constexpr float max_gyro = 5;

        constexpr int CSAG = 21;
        constexpr int CSM = 20;

        constexpr int max_gyro_imu_report_size = 66;
        constexpr int boot_IMU_min_run_time = 20 * time::one_second; // 20 seconds before transition to bootCamera.
        constexpr int door_open_end_time = 19 * time::one_second;    // 19 seconds after the door opens IMU Turn off

        constexpr int sfr_resolution = 10;
    } // namespace imu

    namespace imu_downlink {
        // Note this is how much data correlates to 30 seconds
        constexpr int max_imu_report_length = 1188;
        constexpr uint8_t imu_report_endflag1 = 0xFE;
        constexpr uint8_t imu_report_endflag2 = 0x92;
    } // namespace imu_downlink
    namespace eeprom {
        // Byte locations of the EEPROM blue moon data
        static constexpr unsigned int boot_time_loc1 = 0;
        static constexpr unsigned int boot_counter_loc1 = 4;
        static constexpr unsigned int light_switch_loc1 = 5;
        static constexpr unsigned int dynamic_data_addr_loc1 = 6;
        static constexpr unsigned int sfr_data_addr_loc1 = 8;

        static constexpr unsigned int boot_time_loc2 = 4086;
        static constexpr unsigned int boot_counter_loc2 = 4090;
        static constexpr unsigned int light_switch_loc2 = 4091;
        static constexpr unsigned int dynamic_data_addr_loc2 = 4092;
        static constexpr unsigned int sfr_data_addr_loc2 = 4094;

        // Constants for saving data to EEPROM
        static constexpr unsigned int dynamic_data_full_offset = 8;
        static constexpr unsigned int dynamic_data_start = 10;
        static constexpr unsigned int sfr_data_start = 460;
        static constexpr unsigned int sfr_store_size = 5;
        static constexpr unsigned int sfr_num_fields = 95;
        static constexpr unsigned int sfr_data_full_offset = sfr_num_fields * sfr_store_size + 8;
        static constexpr unsigned int write_age_limit = 95000; // Must be less than 100000

        // Measured in MCL cycles. Fast is ~ every 10 sec, slow is ~ every 60 sec
        static constexpr unsigned int fast_write_interval = 100;
        static constexpr unsigned int slow_write_interval = 600;
    } // namespace eeprom
};    // namespace constants

#endif
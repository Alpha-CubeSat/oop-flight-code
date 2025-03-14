#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <cstddef>
#include <stdint.h>
#include <vector>

namespace constants {
    namespace time {
        constexpr uint32_t one_second = 1000;
        constexpr uint32_t one_minute = 60 * one_second;
        constexpr uint32_t one_hour = 60 * one_minute;
        constexpr uint32_t one_day = 24 * one_hour;
        constexpr uint32_t half_second = one_second / 2;
        constexpr uint32_t one_revolution = 90 * one_minute;

        // control_cycle_time is the value actually used for timing. The
        constexpr uint32_t control_cycle_time_ms = 100;
    } // namespace time
    namespace sensor {
        constexpr int resolution = 1023;
        constexpr float voltage_ref = 3.3;
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
        constexpr int sleep_pin = 19;
        constexpr int min_sleep_period = 2 * time::one_minute;
        constexpr int max_same_mode = (10 * time::one_minute) / time::control_cycle_time_ms;

        constexpr int baud = 19200;
        constexpr size_t buffer_size = 63;
        constexpr size_t num_commas = 5;
        constexpr size_t opcode_len = 2;
        constexpr size_t arg1_len = 4;
        constexpr size_t arg2_len = 4;
        constexpr size_t command_len = opcode_len + arg1_len + arg2_len;

        constexpr int max_imu_report_length = 1188;
        constexpr uint8_t imu_report_endflag1 = 0xFE;
        constexpr uint8_t imu_report_endflag2 = 0x92;

        constexpr uint8_t normal_report_flag = 99;
        constexpr uint8_t imu_report_flag = 24;
        constexpr uint8_t camera_report_flag = 42;

        constexpr uint8_t start_of_command_upload_flag1 = 254;
        constexpr uint8_t start_of_command_upload_flag2 = 254;
        constexpr uint8_t end_of_command_upload_flag1 = 250;
        constexpr uint8_t end_of_command_upload_flag2 = 250;
        namespace opcodes {
            // SFRField Commands
            constexpr uint16_t sfr_field_opcode_min = 0x1100;
            constexpr uint16_t sfr_field_opcode_max = 0x2899;

            // Deploy Command
            constexpr uint16_t deploy_opcode = 0x3333;

            // Arm Command
            constexpr uint16_t arm_opcode = 0x4444;

            // Fire Command
            constexpr uint16_t fire_opcode = 0x5555;

            // EEPROM Reset Command
            constexpr uint16_t eeprom_reset_opcode = 0x7777;

            // Camera Fragment Request Command
            constexpr uint16_t camera_fragment_request_opcode = 0x8888;

            // Mission Mode Override Command
            constexpr uint16_t mission_mode_override_opcode = 0x9999;

            // Fault Suppress Commands
            constexpr uint16_t fault_opcode_min = 0x6000;
            constexpr uint16_t fault_opcode_max = 0x6100;

        } // namespace opcodes
    } // namespace rockblock
    namespace temperature {
        constexpr int pin = 39;
        constexpr int min_temp_c = -100;
        constexpr int max_temp_c = 200;
    } // namespace temperature
    namespace current {
        constexpr int pin = 22;
        constexpr float in_sun_val = 50; // mA
        constexpr int load = 30;         // load resister value (kOhm)
        constexpr float shunt = 0.1;     // shunt resistor value (Ohm)
    } // namespace current
    namespace masks {
        constexpr uint32_t uint32_byte1_mask = 0b11111111000000000000000000000000;
        constexpr uint32_t uint32_byte2_mask = 0b00000000111111110000000000000000;
        constexpr uint32_t uint32_byte3_mask = 0b00000000000000001111111100000000;
        constexpr uint32_t uint32_byte4_mask = 0b00000000000000000000000011111111;

    } // namespace masks
    namespace acs {
        constexpr int xPWMpin = 6; // Flight Unit flipped  X Y
        constexpr int yPWMpin = 10;
        constexpr int zPWMpin = 30;

        constexpr int xout1 = 8;
        constexpr int xout2 = 7;

        constexpr int yout1 = 25;
        constexpr int yout2 = 24;

        constexpr int zout1 = 28;
        constexpr int zout2 = 29;

        constexpr int STBXYpin = 9;
        constexpr int STBZpin = 27;

        // ITS 0 for EDU,  1 for Flight UNIT
        constexpr int xtorqorder = 1;
        constexpr int ytorqorder = 1;
        constexpr int ztorqorder = 1;

        // IMU Offset Coefficients

        // PWM fit coefficients
        // ex: pwmX_ox_1: coefficient for first degree term in pwmX_ox
        constexpr float pwmX_ox_1 = 5.72642735e-02;
        constexpr float pwmX_ox_2 = -3.32941558e-06;
        constexpr float pwmX_ox_3 = 2.51577860e-09;

        constexpr float pwmX_oy_1 = -1.05421663e-02;
        constexpr float pwmX_oy_2 = 1.49763107e-05;
        constexpr float pwmX_oy_3 = -4.08621328e-08;

        constexpr float pwmX_oz_1 = 1.35291840e-03;
        constexpr float pwmX_oz_2 = -4.21466668e-05;
        constexpr float pwmX_oz_3 = -1.38842133e-08;
        ///////////////////////////////
        // Flight Unit:
        constexpr float pwmY_ox_1 = -4.02597445e-02;
        constexpr float pwmY_ox_2 = 2.63292003e-06;
        constexpr float pwmY_ox_3 = 3.38826965e-08;

        constexpr float pwmY_oy_1 = 5.01175923e-02;
        constexpr float pwmY_oy_2 = 9.93774361e-06;
        constexpr float pwmY_oy_3 = -7.85543938e-09;

        constexpr float pwmY_oz_1 = 5.44496439e-02;
        constexpr float pwmY_oz_2 = -4.27335254e-05;
        constexpr float pwmY_oz_3 = 3.92358277e-08;
        /////////////////////////////
        constexpr float pwmZ_ox_1 = 4.44822795e-02;
        constexpr float pwmZ_ox_2 = -2.03606573e-06;
        constexpr float pwmZ_ox_3 = -9.01969790e-09;

        constexpr float pwmZ_oy_1 = 7.50075982e-03;
        constexpr float pwmZ_oy_2 = 2.31807375e-05;
        constexpr float pwmZ_oy_3 = 6.45761942e-09;

        constexpr float pwmZ_oz_1 = 1.50101720e-02;
        constexpr float pwmZ_oz_2 = -4.75292304e-05;
        constexpr float pwmZ_oz_3 = 2.72118450e-08;
        ///////////////////////////
        ///////////////////////////
        // Voltage fit coefficients
        // ex: volX_ox_1, volX_ox_c --> coefficient and constant respectively for volX_ox
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
        constexpr float hardiron_x = 31.5;
        constexpr float hardiron_y = 14.5;
        constexpr float hardiron_z = -6.0;

        // Starshot
        constexpr float step_size_input = 0.10;
        constexpr float A_input = 4.0E-5;
        constexpr float i_max_input = 0.25;
        constexpr float k_input = 13.5;
        constexpr float n_input = 500.0;
        constexpr float Kd_values[31] = {0.0007935, 0.0099, 0.01485, 0.0198, 0.02475, 0.0297, 0.03465, 0.0396, 0.04455, 0.0495, 0.05445, 0.0594, 0.06435, 0.0693, 0.07425, 0.0792, 0.08415, 0.0891, 0.09405, 0.099, 0., 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.};
        constexpr float Kp_values[31] = {5.250E-10, 9.9900E-09, 1.4985E-08, 1.9980E-08, 2.4975E-08, 2.9970E-08, 3.4965E-08, 3.9960E-08, 4.4955E-08, 4.9950E-08, 5.4945E-08, 5.9940E-08, 6.4935E-08, 6.9930E-08, 7.4925E-08, 7.9920E-08, 8.4915E-08, 8.9910E-08, 9.4905E-08, 9.9900E-08, 0.0000e+00, 1.0000E-01, 2.0000E-01, 3.0000E-01, 4.0000E-01, 5.0000E-01, 6.0000E-01, 7.0000E-01, 8.0000E-01, 9.0000E-01, 1.0000e+00};
        constexpr float Id_values[31] = {0.196, 1.0, 1.6681005372000588, 2.7825594022071245, 4.641588833612778, 7.742636826811269, 12.91549665014884, 21.544346900318832, 35.93813663804626, 59.94842503189409, 100.0, 0.01, 0.0206913808111479, 0.04281332398719394, 0.08858667904100823, 0.18329807108324356, 0.37926901907322497, 0.7847599703514611, 1.623776739188721, 3.359818286283781, 6.951927961775605, 14.38449888287663, 29.76351441631316, 61.584821106602604, 127.42749857031322, 263.6650898730355, 545.5594781168514, 1128.8378916846884, 2335.7214690901214, 4832.930238571752, 10000.0};
        constexpr float c_values[31] = {1.0E-5, 2.0000000000000003e-06, 3.3362010744001187e-06, 5.565118804414253e-06, 9.283177667225566e-06, 1.5485273653622557e-05, 2.5830993300297657e-05, 4.308869380063765e-05, 7.187627327609252e-05, 0.00011989685006378819, 0.00020000000000000004, 1e-07, 2.6366508987303554e-07, 6.951927961775605e-07, 1.8329807108324375e-06, 4.832930238571752e-06, 1.274274985703132e-05, 3.359818286283781e-05, 8.858667904100833e-05, 0.00023357214690901214, 0.0006158482110660261, 0.001623776739188721, 0.004281332398719396, 0.011288378916846883, 0.02976351441631313, 0.07847599703514607, 0.20691380811147903, 0.5455594781168515, 1.43844988828766, 3.792690190732246, 10.0};
        constexpr float spin_resolution = 10;

        constexpr float current_resolution = 100;
        constexpr float current_offset = -0.25;
    } // namespace acs
    namespace battery {
        constexpr int voltage_value_pin = 32;
        constexpr int r1 = 4700;
        constexpr int r2 = 10000;
        constexpr int min_voltage = 0;
        constexpr int max_voltage = 6;
        constexpr int sfr_resolution = 100;
    } // namespace battery
    namespace camera {
        constexpr int power_on_pin = 31;
        constexpr int content_length = 80;
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

        constexpr float min_gyro = -10;
        constexpr float max_gyro = 10;

        constexpr int CSAG = 21;
        constexpr int CSM = 20;

        constexpr uint8_t max_gyro_imu_report_size = 66;
        constexpr uint32_t boot_IMU_min_run_time = 20 * time::one_second; // 20 seconds before transition to bootCamera.
        constexpr uint32_t door_open_end_time = 10 * time::one_second;    // 10 seconds after the door opens IMU Turn off

        constexpr int sfr_resolution = 10;
    } // namespace imu

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
        static constexpr unsigned int sfr_num_fields = 94;
        static constexpr unsigned int sfr_data_full_offset = sfr_num_fields * sfr_store_size + 4;
        static constexpr unsigned int write_age_limit = 95000; // Must be less than 100000

        // Measured in MCL cycles
        static constexpr unsigned int fast_write_interval = 100;
        static constexpr unsigned int slow_write_interval = 600;
    } // namespace eeprom
    namespace watchdog {
        constexpr uint32_t max_period_ms = 10000;
    } // namespace watchdog
}; // namespace constants

#endif
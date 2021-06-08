#include "sfr.hpp"

namespace sfr
{
    namespace photoresistor
    {
        bool covered = true;
    }
    namespace mission
    {
        mission_mode_type mode = mission_mode_type::standby;
    }
    namespace burnwire
    {
        bool fire = false;
        bool arm = false;
        burnwire_mode_type mode = burnwire_mode_type::standby;
        int attempts = 0;
        int start_time = 0;
    }
    namespace camera
    {
        bool take_photo = false;
        bool turn_on = false;
        bool turn_off = false;
        bool powered = false;
        uint8_t buffer[255] = {0};
        uint16_t current_serial = 0;
        uint8_t fragment_number = 0;
        uint8_t max_fragments = 0;
        uint8_t data_length = 0;
        uint8_t fragment_number_requested = 3;
        uint8_t serial_requested = 0;
        bool fragment_requested = false;
        uint8_t images_written = 0;
        uint16_t image_lengths[255];
        bool report_ready = false;
        bool sd_card_failed = false;
        bool camera_failed = false;
        bool full_image_written = false;
        bool report_downlinked = true;
        HardwareSerial serial = Serial5;
    }
    namespace rockblock
    {
        unsigned long last_downlink = 0;
        unsigned long downlink_period = constants::rockblock::two_hours;
        rockblock_mode_type mode = rockblock_mode_type::send_at;
        bool waiting_message = false;
        char buffer[constants::rockblock::buffer_size] = {0};
        uint8_t report[constants::rockblock::packet_size] = {0};
        int commas[constants::rockblock::num_commas] = {0};
        unsigned char opcode[2] = {0};
        unsigned char arg_1[4] = {0};
        unsigned char arg_2[4] = {0};
        HardwareSerial serial = Serial1;
        int num_iter = 0;
        bool waiting_command = false;
        int f_opcode = 0;
        int f_arg_1 = 0;
        int f_arg_2 = 0;
    }
    namespace imu
    {
        float mag_x = 0.0;
        float mag_y = 0.0;
        float mag_z = 0.0;
        float gyro_x = 0.0;
        float gyro_y = 0.0;
        float gyro_z = 0.0;
        float acc_x = 0.0;
        float acc_y = 0.0;
        float acc_z = 0.0;
        
        float mag_x_average = 0.0;
        float mag_y_average = 0.0;
        float mag_z_average = 0.0;
        float gyro_x_average = 0.0;
        float gyro_y_average = 0.0;
        float gyro_z_average = 0.0;
        float acc_x_average = 0.0;
        float acc_y_average = 0.0;
        float acc_z_average = 0.0;
    }
    namespace temperature
    {
        float temp_c = 0.0;
        float temp_c_average = 0.0;
        temp_mode_type mode = temp_mode_type::active;
    }
    namespace current
    {
        float solar_current = 0.0;
        float solar_current_average = 0.0;
        bool in_sun = false;
    }
    namespace acs
    {
        acs_mode_type mode = acs_mode_type::detumble;
        int current1 = 0;
        int current2 = 0;
        int current3 = 0;
    }
    namespace battery
    {
        float voltage = 0.0;
        float voltage_average = 0.0;
    }
    namespace fault
    {
        fault_mode_type mode = fault_mode_type::active;

        unsigned char fault_1 = 0;
        unsigned char fault_2 = 0;
        unsigned char fault_3 = 0;

        //FAULT 1
        bool check_mag_x = true;
        bool check_mag_y = true;
        bool check_mag_z = true;
        bool check_gyro_x = true;
        bool check_gyro_y = true;
        bool check_gyro_z = true;
        bool check_acc_x = true;
        bool check_acc_y = true;

        //FAULT 2
        bool check_acc_z = true;
        bool check_temp_c = true;
        bool check_solar_current = true;
        bool check_voltage = true;
    }
    namespace button
    {
        bool pressed = true;
    }
}
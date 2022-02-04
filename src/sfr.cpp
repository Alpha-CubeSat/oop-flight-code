#include "sfr.hpp"

namespace sfr {
    namespace pins {
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
    }
    namespace photoresistor {
        bool covered = true;
    }
    namespace mission {
        mission_mode_type mode = mission_mode_type::boot;
        bool low_power_eligible = true;
        unsigned long boot_start_test = 0;

    } // namespace mission
    namespace burnwire {
        bool fire = false;
        bool arm = false;
        burnwire_mode_type mode = burnwire_mode_type::standby;
        int attempts = 0;
        int start_time = 0;
        int camera_attempts = 0;
        int burn_time = 500;
        int armed_time = constants::time::two_days;
    } // namespace burnwire
    namespace camera {
        bool photo_taken_sd_failed = false;
        bool take_photo = false;
        bool turn_on = false;
        bool turn_off = false;
        bool powered = false;
        bool begun = false;
        bool resolution_set = false;
        uint64_t start_time = 0;
        uint64_t begin_time = 0;
        uint64_t resolution_set_time = 0;
        uint64_t buffer[255] = {0};
        int current_serial = 0;
        int fragment_number = 0;
        int fragment_number_requested = 3;
        int serial_requested = 0;
        bool fragment_requested = false;
        int images_written = 0;
        int fragments_written = 0;
        int image_lengths[255];
        bool report_ready = false;
        bool full_image_written = false;
        bool report_downlinked = true;
        char filename[15];
        uint16_t jpglen = 0;
        uint8_t set_res = VC0706_160x120;
    } // namespace camera
    namespace rockblock {
        unsigned long last_communication = 0;
        bool last_downlink_normal = false;
        int camera_commands[99][constants::rockblock::command_len] = {};
        int camera_max_fragments[99] = {};
        bool downlink_camera = false;
        unsigned long last_downlink = 0;
        unsigned long downlink_period = 0;
        unsigned long camera_downlink_period = 0;
        rockblock_mode_type mode = rockblock_mode_type::send_at;
        bool waiting_message = false;
        char buffer[constants::rockblock::buffer_size] = {0};
        uint8_t report[constants::rockblock::packet_size] = {0};
        uint8_t camera_report[constants::rockblock::packet_size] = {0};
        int commas[constants::rockblock::num_commas] = {0};
        uint8_t opcode[2] = {0};
        uint8_t arg_1[4] = {0};
        uint8_t arg_2[4] = {0};
#ifndef SIMULATOR
        HardwareSerial serial = Serial1;
#else
        RockblockSimulator serial;
#endif
        bool flush_status = false;
        bool waiting_command = false;
        size_t conseq_reads = 0;
        uint16_t f_opcode = 0;
        uint32_t f_arg_1 = 0;
        uint32_t f_arg_2 = 0;
        int timeout = 10 * constants::time::one_minute;
        int start_time = 0;
        bool last_timed_out = false;
        int num_downlinks = 0;
    } // namespace rockblock
    namespace imu {
        float mag_x = 0.0;
        float mag_y = 0.0;
        float mag_z = 0.0;
        float gyro_x = 0.0;
        float gyro_y = 0.0;
        float gyro_z = 0.0;

        std::deque<float> mag_x_buffer;
        std::deque<float> mag_y_buffer;
        std::deque<float> mag_z_buffer;
        std::deque<float> gyro_x_buffer;
        std::deque<float> gyro_y_buffer;
        std::deque<float> gyro_z_buffer;
        std::deque<float> acc_x_buffer;
        std::deque<float> acc_y_buffer;
        std::deque<float> acc_z_buffer;

        std::deque<float> gyro_x_dlink_buffer;
        std::deque<float> gyro_y_dlink_buffer;
        std::deque<float> gyro_z_dlink_buffer;

        float mag_x_average = 0.0;
        float mag_y_average = 0.0;
        float mag_z_average = 0.0;
        float gyro_x_average = 0.0;
        float gyro_y_average = 0.0;
        float gyro_z_average = 0.0;
        float acc_x_average = 0.0;
        float acc_y_average = 0.0;
        float acc_z_average = 0.0;
    } // namespace imu
    namespace temperature {
        float temp_c = 0.0;
        std::deque<float> temp_c_buffer;
        float temp_c_average = 0.0;
        bool in_sun = false;
    } // namespace temperature
    namespace current {
        float solar_current = 0.0;
        std::deque<float> solar_current_buffer;
        float solar_current_average = 0.0;
        bool in_sun = false;
    } // namespace current
    namespace acs {
        acs_mode_type mode = acs_mode_type::off;
        float current1 = 0;
        float current2 = 0;
        float current3 = 0;
        simple_acs_type mag = simple_acs_type::x;
        unsigned long max_no_communication = 0;
    } // namespace acs
    namespace battery {
        float voltage = 0.0;
        std::deque<float> voltage_buffer;
        float voltage_average = 0.0;
    } // namespace battery
    namespace fault {
        fault_mode_type mode = fault_mode_type::active;

        unsigned char fault_1 = 0;
        unsigned char fault_2 = 0;
        unsigned char fault_3 = 0;

        // FAULT 1
        bool check_mag_x = true;
        bool check_mag_y = true;
        bool check_mag_z = true;
        bool check_gyro_x = true;
        bool check_gyro_y = true;
        bool check_gyro_z = true;
        bool check_acc_x = true;
        bool check_acc_y = true;

        // FAULT 2
        bool check_acc_z = true;
        bool check_temp_c = true;
        bool check_solar_current = true;
        bool check_voltage = true;
    } // namespace fault
    namespace button {
        bool pressed = true;
    }
} // namespace sfr
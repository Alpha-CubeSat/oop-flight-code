#include "sfr.hpp"

namespace sfr {
    namespace boot {
        unsigned long max_time = constants::time::two_hours;
    }
    namespace simple {
        float max_time = 5 * constants::time::one_minute;
    }
    namespace point {
        float max_time = 5 * constants::time::one_minute;
    }
    namespace detumble {
        float start_time = 0;
        float max_time = constants::time::two_hours;
        // TODO
        int num_imu_retries = 0;
        int max_imu_retries = 5;

        float min_stable_gyro_z = 1;
        float max_stable_gyro_x = 0.2;
        float max_stable_gyro_y = 0.2;

        float min_unstable_gyro_x = 0.7;
        float min_unstable_gyro_y = 0.7;
    } // namespace detumble
    namespace aliveSignal {
        int max_downlink_hard_faults = 3;
        bool downlinked = false;
        float max_time = constants::time::two_hours;
        int num_hard_faults = 0;
    } // namespace aliveSignal
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
    } // namespace pins
    namespace photoresistor {
        int val = 0;
        bool covered = true;
    } // namespace photoresistor
    namespace mission {
        Boot boot_class;
        AliveSignal aliveSignal_class;
        LowPowerAliveSignal lowPowerAliveSignal_class;
        DetumbleSpin detumbleSpin_class;
        LowPowerDetumbleSpin lowPowerDetumbleSpin_class;
        Normal normal_class;
        Transmit transmit_class;
        LowPower lowPower_class;
        NormalDeployment normalDeployment_class;
        TransmitDeployment transmitDeployment_class;
        LowPowerDeployment lowPowerDeployment_class;
        NormalArmed normalArmed_class;
        TransmitArmed transmitArmed_class;
        LowPowerArmed lowPowerArmed_class;
        NormalInSun normalInSun_class;
        TransmitInSun transmitInSun_class;
        LowPowerInSun lowPowerInSun_class;
        VoltageFailureInSun voltageFailureInSun_class;
        BootCamera bootCamera_class;
        MandatoryBurns mandatoryBurns_class;
        RegularBurns regularBurns_class;
        Photo photo_class;

        MissionMode *boot = &boot_class;
        MissionMode *aliveSignal = &aliveSignal_class;
        MissionMode *lowPowerAliveSignal = &lowPowerAliveSignal_class;
        MissionMode *detumbleSpin = &detumbleSpin_class;
        MissionMode *lowPowerDetumbleSpin = &lowPowerDetumbleSpin_class;
        MissionMode *normal = &normal_class;
        MissionMode *transmit = &transmit_class;
        MissionMode *lowPower = &lowPower_class;
        MissionMode *normalDeployment = &normalDeployment_class;
        MissionMode *transmitDeployment = &transmitDeployment_class;
        MissionMode *lowPowerDeployment = &lowPowerDeployment_class;
        MissionMode *normalArmed = &normalArmed_class;
        MissionMode *transmitArmed = &transmitArmed_class;
        MissionMode *lowPowerArmed = &lowPowerArmed_class;
        MissionMode *normalInSun = &normalInSun_class;
        MissionMode *transmitInSun = &transmitInSun_class;
        MissionMode *lowPowerInSun = &lowPowerInSun_class;
        MissionMode *voltageFailureInSun = &voltageFailureInSun_class;
        MissionMode *bootCamera = &bootCamera_class;
        MissionMode *mandatoryBurns = &mandatoryBurns_class;
        MissionMode *regularBurns = &regularBurns_class;
        MissionMode *photo = &photo_class;

        MissionMode *current_mode = boot;
        MissionMode *previous_mode = boot;

        std::deque<int> mode_history;

        float acs_transmit_cycle_time = constants::time::one_minute * 100;
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
        sensor_mode_type mode = sensor_mode_type::normal;
        bool photo_taken_sd_failed = false;
        bool take_photo = false;
        bool turn_on = false;
        bool turn_off = false;
        bool powered = false;

        // Initialization
        camera_init_mode_type init_mode = camera_init_mode_type::awaiting;
        uint8_t start_progress = 0;
        uint64_t step_time = 0;
        uint64_t init_start_time = 0;
        uint64_t init_timeout = 12000;
        uint8_t begin_delay = 100;
        uint8_t resolution_set_delay = 500;
        uint8_t resolution_get_delay = 200;

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
        report_type downlink_report_type = report_type::normal_report;
        bool rockblock_ready_status = false;
        rockblock_mode_type mode = rockblock_mode_type::send_at;

        unsigned long last_downlink = 0;
        unsigned long downlink_period = 0;

        bool waiting_message = false;

        std::deque<uint8_t> downlink_report;
        std::deque<uint8_t> normal_report;
        std::deque<uint8_t> camera_report;
        uint8_t imu_report[constants::rockblock::packet_size] = {0};

        char buffer[constants::rockblock::buffer_size] = {0};
        int camera_commands[99][constants::rockblock::command_len] = {};
        int camera_max_fragments[99] = {};
        int commas[constants::rockblock::num_commas] = {0};

        uint8_t opcode[2] = {0};
        uint8_t arg_1[4] = {0};
        uint8_t arg_2[4] = {0};

        int imu_max_fragments = 10;
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
        int num_downlinks = 2;
        float start_time_check_signal = 0;
        float max_check_signal_time = constants::time::one_minute;
    } // namespace rockblock
    namespace imu {
        sensor_mode_type mode = sensor_mode_type::init;
        bool successful_init = true;

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
        // std::deque<std::experimental::any, time_t> imu_dlink_buffer;
        std::deque<float> imu_dlink_gyro_x_buffer;
        std::deque<float> imu_dlink_gyro_y_buffer;
        std::deque<float> imu_dlink_gyro_z_buffer;

        SensorReading *mag_x_average = new SensorReading(fault_index_type::mag_x, 0.0, false);
        SensorReading *mag_y_average = new SensorReading(fault_index_type::mag_y, 0.0, false);
        SensorReading *mag_z_average = new SensorReading(fault_index_type::mag_z, 0.0, false);
        SensorReading *gyro_x_average = new SensorReading(fault_index_type::gyro_x, 0.0, false);
        SensorReading *gyro_y_average = new SensorReading(fault_index_type::gyro_y, 0.0, false);
        SensorReading *gyro_z_average = new SensorReading(fault_index_type::gyro_z, 0.0, false);
        SensorReading *acc_x_average = new SensorReading(fault_index_type::acc_x, 0.0, false);
        SensorReading *acc_y_average = new SensorReading(fault_index_type::acc_y, 0.0, false);

        imu_downlink_type imu_dlink_magid = imu_downlink_type::GAUSS_8;
        const int imu_downlink_buffer_max_size = constants::sensor::collect; // not determined yet
        const int imu_downlink_report_size = constants::sensor::collect * 5;
        uint8_t imu_downlink_report[imu_downlink_report_size];

        int fragment_number = 0;
        bool fragment_requested = false;
        int fragments_written = 0;
        bool imu_dlink_report_ready = false;
        bool report_downlinked = true;
        char filename[15];

        const int mag_8GAUSS_min = 4;
        const int mag_12GAUSS_min = 8;
        const int mag_16GAUSS_min = 12;
        const int gyro_500DPS_min = 245;
        const int gyro_2000DPS_min = 500;

        bool sample = true;
    } // namespace imu
    namespace temperature {
        float temp_c = 0.0;
        std::deque<float> temp_c_buffer;
        SensorReading *temp_c_average = new SensorReading(fault_index_type::temp_c, 0.0, false);
        bool in_sun = false;
    } // namespace temperature
    namespace current {
        float solar_current = 0.0;
        std::deque<float> solar_current_buffer;
        SensorReading *solar_current_average = new SensorReading(fault_index_type::solar_current, 0.0, false);
        bool in_sun = false;
    } // namespace current

    namespace acs {
        Simple simple_class;
        Point point_class;
        Off off_class;

        ACSMode *simple = &simple_class;
        ACSMode *point = &point_class;
        ACSMode *off = &off_class;

        ACSMode *current_mode = off;

        float current1 = 0;
        float current2 = 0;
        float current3 = 0;
        float pwm1 = 0;
        float pwm2 = 0;
        float pwm3 = 0;
        simple_acs_type mag = simple_acs_type::x;
        unsigned long max_no_communication = 0;

        float on_time = 5 * constants::time::one_minute;
    } // namespace acs
    namespace battery {
        float voltage = 0.0;
        std::deque<float> voltage_buffer;
        SensorReading *voltage_average = new SensorReading(fault_index_type::voltage, 0.0, false);
        // TODO
        float acceptable_battery;
        float min_battery;
    } // namespace battery
    namespace fault {
        fault_mode_type mode = fault_mode_type::active;

        // unsigned char fault_1 = 0;
        // unsigned char fault_2 = 0;
        // unsigned char fault_3 = 0;

        // // FAULT 1
        // bool check_mag_x = true;
        // bool check_mag_y = true;
        // bool check_mag_z = true;
        // bool check_gyro_x = true;
        // bool check_gyro_y = true;
        // bool check_gyro_z = true;
        // bool check_acc_x = true;
        // bool check_acc_y = true;

        // // FAULT 2
        // bool check_acc_z = true;
        // bool check_temp_c = true;
        // bool check_solar_current = true;
        // bool check_voltage = true;

        // // FAULT 3
        // bool check_burn_wire = true;
        // bool check_sd_card = true;
        // bool check_camera_on_failed = true;
    } // namespace fault
    namespace button {
        bool pressed = true;
    }
} // namespace sfr
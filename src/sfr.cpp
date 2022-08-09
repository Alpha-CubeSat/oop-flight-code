#include "sfr.hpp"

namespace sfr {
    namespace stabilization {
        SFRField<float> max_time(30 * constants::time::one_minute, 0, 5 * constants::time::one_hour, 1001);
    } // namespace stabilization
    namespace boot {
        SFRField<float> max_time(2 * constants::time::one_hour, 10, 5 * constants::time::one_hour, 1002);
    }
    namespace simple {
        SFRField<float> max_time(5 * constants::time::one_minute, 10, 5 * constants::time::one_hour, 1003);
    }
    namespace point {
        float max_time = 5 * constants::time::one_minute;
    }
    namespace detumble {
        float start_time = 0;
        float max_time = 2 * constants::time::one_hour;
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
        float max_time = 2 * constants::time::one_hour;
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
        std::deque<int> light_val_buffer;
        SensorReading *light_val_average = new SensorReading(fault_index_type::light_val, 0.0, false);
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

        Initialization initialization_class;
        Stabilization stabilization_class;
        Standby standby_class;
        Deployment deployment_class;
        Armed armed_class;
        InSun insun_class;
        Firing firing_class;

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

        Phase *initialization = &initialization_class;
        Phase *stabilization = &stabilization_class;
        Phase *standby = &standby_class;
        Phase *deployment = &deployment_class;
        Phase *armed = &armed_class;
        Phase *inSun = &insun_class;
        Phase *firing = &firing_class;

        MissionMode *current_mode = boot;
        MissionMode *previous_mode = boot;

        Phase *current_phase = initialization;
        Phase *previous_phase = initialization;

        std::deque<int> mode_history;

        float acs_transmit_cycle_time = constants::time::one_minute * 100;

        float time_deployed;
        bool deployed;
        bool already_deployed;
    } // namespace mission
    namespace burnwire {
        bool fire = false;
        bool arm = false;
        burnwire_mode_type mode = burnwire_mode_type::standby;
        int attempts = 0;
        int attempts_limit = 10;
        int start_time = 0;
        int camera_attempts = 0;
        int burn_time = 500;
        int armed_time = 2 * constants::time::one_day;
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
        uint8_t failed_times = 0;
        uint8_t failed_limit = 5;
        uint64_t step_time = 0;
        uint64_t init_start_time = 0;
        uint64_t init_timeout = 12000;
        uint8_t begin_delay = 100;
        uint16_t resolution_set_delay = 500;
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
        std::deque<uint8_t> imu_report;

        char buffer[constants::rockblock::buffer_size] = {0};
        int camera_commands[99][constants::rockblock::command_len] = {};
        int camera_max_fragments[99] = {};
        int commas[constants::rockblock::num_commas] = {0};

        std::deque<RawRockblockCommand> raw_commands;
        std::deque<RockblockCommand> processed_commands;
        int max_commands_count = 10;

        int imu_downlink_max_fragments[99] = {};
        int imu_max_fragments = 256;

        float imudownlink_start_time = 0.0;
        float imudownlink_remain_time = constants::time::one_minute;
        bool imu_first_start = true;
        bool imu_downlink_on = true;
#ifndef SIMULATOR
        HardwareSerial serial = Serial1;
#else
        RockblockSimulator serial;
#endif
        bool flush_status = false;
        bool waiting_command = false;
        size_t conseq_reads = 0;
        int timeout = 10 * constants::time::one_minute;
        int start_time = 0;
        float start_time_check_signal = 0;
        float max_check_signal_time = constants::time::one_minute;
        bool sleep_mode = false;
    } // namespace rockblock
    namespace imu {
        sensor_mode_type mode = sensor_mode_type::init;
        bool successful_init = true;
        bool turn_off = false; // action variable
        bool turn_on = false;  // action variable
        bool powered = false;  // state variable

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
        std::deque<uint8_t> imu_dlink;

        SensorReading *mag_x_average = new SensorReading(fault_index_type::mag_x, 0.0, false);
        SensorReading *mag_y_average = new SensorReading(fault_index_type::mag_y, 0.0, false);
        SensorReading *mag_z_average = new SensorReading(fault_index_type::mag_z, 0.0, false);
        SensorReading *gyro_x_average = new SensorReading(fault_index_type::gyro_x, 0.0, false);
        SensorReading *gyro_y_average = new SensorReading(fault_index_type::gyro_y, 0.0, false);
        SensorReading *gyro_z_average = new SensorReading(fault_index_type::gyro_z, 0.0, false);
        SensorReading *acc_x_average = new SensorReading(fault_index_type::acc_x, 0.0, false);
        SensorReading *acc_y_average = new SensorReading(fault_index_type::acc_y, 0.0, false);

        SensorReading *gyro_x_value = new SensorReading(fault_index_type::gyro_x, 0.0, false);
        SensorReading *gyro_y_value = new SensorReading(fault_index_type::gyro_y, 0.0, false);
        SensorReading *gyro_z_value = new SensorReading(fault_index_type::gyro_z, 0.0, false);

        // check with Josh
        int gyro_min = -245;
        int gyro_max = 245;
        int mag_min = -8;
        int mag_max = 8;

        bool start_timing_deployed = false;
        float start_time_deployed = 0.0;
        uint8_t current_sample = 0;
        bool sample_gyro = false;
        uint8_t fragment_number = 0;
        bool report_ready = false;
        bool report_downlinked = true;
        bool report_written = false;
        bool full_report_written = false;
        int max_fragments = 256;
        int content_length = 68;

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
        float current1 = 0;
        float current2 = 0;
        float current3 = 0;
        float pwm1 = 0;
        float pwm2 = 0;
        float pwm3 = 0;
        simple_acs_type mag = simple_acs_type::x;
        unsigned long max_no_communication = 0;

        float on_time = 5 * constants::time::one_minute;
        bool off = true;
    } // namespace acs
    namespace battery {
        float voltage = 0.0;
        std::deque<float> voltage_buffer;
        SensorReading *voltage_average = new SensorReading(fault_index_type::voltage, 0.0, false);
        // TODO
        float acceptable_battery = 3.9;
        float min_battery = 3.75;
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
    namespace EEPROM {
        int time_of_last_write = 0;
        int write_step_time = 1000; // the amount of time between each write to EEPROM
        int alloted_time = 7200000; // the amount of time for the EEPROM to count to (7200000 ms = 2 h)
        int eeprom_value = 0;       // the amount of time that the EEPROM has counted, stops when the alloted time has been reached
        bool alloted_time_passed = false;
    } // namespace EEPROM
} // namespace sfr
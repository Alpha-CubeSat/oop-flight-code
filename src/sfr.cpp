#include "sfr.hpp"

namespace sfr {
    namespace stabilization {
        // OP Codes 1100`
        SFRField<uint32_t> max_time = SFRField<uint32_t>(30 * constants::time::one_minute, 0UL, 5 * constants::time::one_hour, 0x1100, constants::eeprom::stabilization_max_time_offset, true);
    } // namespace stabilization
    namespace boot {
        // OP Codes 1200
        SFRField<uint32_t> max_time = SFRField<uint32_t>(2 * constants::time::one_hour, 10UL, 5 * constants::time::one_hour, 0x1200, constants::eeprom::boot_max_time_offset, true);
    } // namespace boot
    namespace simple {
        // OP Codes 1300
        SFRField<uint32_t> max_time = SFRField<uint32_t>(5 * constants::time::one_minute, 10UL, 5 * constants::time::one_hour, 0x1300, constants::eeprom::simple_max_time_offset, true);
    } // namespace simple
    namespace point {
        // OP Codes 1400
        SFRField<uint32_t> max_time = SFRField<uint32_t>(5 * constants::time::one_minute, 0x1400, constants::eeprom::point_max_time_offset, true);
    } // namespace point
    namespace detumble {
        // OP Codes 1500
        SFRField<uint32_t> start_time = SFRField<uint32_t>(0UL, 0x1500, constants::eeprom::detumble_start_time_offset, true);
        SFRField<uint32_t> max_time = SFRField<uint32_t>(2 * constants::time::one_hour, 0x1501, constants::eeprom::detumble_max_time_offset, true);

        SFRField<uint8_t> min_stable_gyro_z = SFRField<uint8_t>(1, 0, 2, 0x1504, 10, constants::eeprom::detumble_min_stable_gyro_z_offset, true);
        SFRField<uint8_t> max_stable_gyro_x = SFRField<uint8_t>(.2, 0, 1, 0x1505, 10, constants::eeprom::detumble_max_stable_gyro_x_offset, true);
        SFRField<uint8_t> max_stable_gyro_y = SFRField<uint8_t>(.2, 0, 1, 0x1506, 10, constants::eeprom::detumble_max_stable_gyro_y_offset, true);

        SFRField<uint8_t> min_unstable_gyro_x = SFRField<uint8_t>(.7, 0, 1, 0x1507, 10, constants::eeprom::detumble_min_unstable_gyro_x_offset, true);
        SFRField<uint8_t> min_unstable_gyro_y = SFRField<uint8_t>(.7, 0, 1, 0x1508, 10, constants::eeprom::detumble_min_unstable_gyro_y_offset, true);
    } // namespace detumble
    namespace aliveSignal {
        // OP Codes 1600
        SFRField<uint16_t> max_downlink_hard_faults = SFRField<uint16_t>(3, 0x1600, constants::eeprom::aliveSignal_max_downlink_hard_faults_offset, true);
        SFRField<bool> downlinked = SFRField<bool>(false, 0x1601, constants::eeprom::aliveSignal_downlinked_offset, true);
        SFRField<uint32_t> max_time = SFRField<uint32_t>(2 * constants::time::one_hour, 0x1602, constants::eeprom::aliveSignal_max_time_offset, true);
        SFRField<uint16_t> num_hard_faults = SFRField<uint16_t>(0, 0x1603, constants::eeprom::aliveSignal_num_hard_faults_offset, true);
    } // namespace aliveSignal
    namespace photoresistor {
        // OP Codes 1700
        SFRField<bool> covered = SFRField<bool>(true, 0x1700, constants::eeprom::photoresistor_covered_offset, true);

        SensorReading *light_val_average_standby = new SensorReading(fault_index_type::light_val, 20, 0, 1000);
        SensorReading *light_val_average_deployment = new SensorReading(fault_index_type::light_val, 1, 0, 1000);
    } // namespace photoresistor
    namespace mission {
        // OP Codes 1800
        SFRField<uint32_t> acs_transmit_cycle_time = SFRField<uint32_t>(100 * constants::time::one_minute, 0x1800, constants::eeprom::mission_acs_transmit_cycle_time_offset, true);

        SFRField<uint32_t> time_deployed = SFRField<uint32_t>(0, 0x1801, constants::eeprom::mission_time_deployed_offset, true);
        SFRField<bool> deployed = SFRField<bool>(false, 0x1802, constants::eeprom::mission_deployed_offset, true);
        SFRField<bool> already_deployed = SFRField<bool>(false, 0x1803, constants::eeprom::mission_already_deployed_offset, true);
        SFRField<bool> possible_uncovered = SFRField<bool>(false, 0x1804, constants::eeprom::mission_possible_uncovered_offset, true);

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
        BootIMU bootImu_class;
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
        MissionMode *bootImu = &bootImu_class;
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
    } // namespace mission
    namespace burnwire {
        // OP Codes 1900
        SFRField<uint16_t> attempts = SFRField<uint16_t>(0, 0x1900, constants::eeprom::burnwire_attempts_offset, true);
        SFRField<uint32_t> start_time = SFRField<uint32_t>(0, 0x1901, constants::eeprom::burnwire_start_time_offset, true);
        SFRField<uint32_t> burn_time = SFRField<uint32_t>(600, 0x1902, constants::eeprom::burnwire_burn_time_offset, true);
        SFRField<uint32_t> armed_time = SFRField<uint32_t>(2 * constants::time::one_day, 0x1903, constants::eeprom::burnwire_armed_time_offset, true);
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)burnwire_mode_type::standby, 0x1904, constants::eeprom::burnwire_mode_offset, true);
        // 1 attempt = 1 burn of 1 wire. Therefore, double the number of attempts if you want an "attempt" to burn both wires
        SFRField<uint16_t> attempts_limit = SFRField<uint16_t>(10, 0x1905, constants::eeprom::burnwire_attempts_limit_offset, true);
        SFRField<uint16_t> mandatory_attempts_limit = SFRField<uint16_t>(4, 0x1906, constants::eeprom::burnwire_mandatory_attempts_limit_offset, true);
        SFRField<uint32_t> delay_time = SFRField<uint32_t>(1000, 0x1907, constants::eeprom::burnwire_delay_time_offset, true);
    } // namespace burnwire
    namespace camera {
        // OP Codes 2000
        SFRField<bool> photo_taken_sd_failed = SFRField<bool>(false, 0x2000, constants::eeprom::camera_photo_taken_sd_failed_offset, true);
        SFRField<bool> take_photo = SFRField<bool>(false, 0x2001, constants::eeprom::camera_take_photo_offset, true);
        SFRField<bool> turn_on = SFRField<bool>(false, 0x2002, constants::eeprom::camera_turn_on_offset, true);
        SFRField<bool> turn_off = SFRField<bool>(false, 0x2003, constants::eeprom::camera_turn_off_offset, true);
        SFRField<bool> powered = SFRField<bool>(false, 0x2004, constants::eeprom::camera_powered_offset, true);

        // Initialization
        SFRField<uint8_t> start_progress = SFRField<uint8_t>(0, 0x2005, constants::eeprom::camera_start_progress_offset, true);
        SFRField<uint32_t> step_time = SFRField<uint32_t>(0, 0x2006, constants::eeprom::camera_step_time_offset, true);
        SFRField<uint32_t> init_start_time = SFRField<uint32_t>(0, 0x2007, constants::eeprom::camera_init_start_time_offset, true);
        SFRField<uint32_t> init_timeout = SFRField<uint32_t>(12000, 0x2008, constants::eeprom::camera_init_timeout_offset, true);
        SFRField<uint32_t> begin_delay = SFRField<uint32_t>(100, 0x2009, constants::eeprom::camera_begin_delay_offset, true);
        SFRField<uint32_t> resolution_set_delay = SFRField<uint32_t>(500, 0x2010, constants::eeprom::camera_resolution_set_delay_offset, true);
        SFRField<uint32_t> resolution_get_delay = SFRField<uint32_t>(200, 0x2011, constants::eeprom::camera_resolution_get_delay_offset, true);

        SFRField<uint16_t> init_mode = SFRField<uint16_t>((uint16_t)sensor_init_mode_type::awaiting, 0x2012, constants::eeprom::camera_init_mode_offset, true);
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)sensor_mode_type::normal, 0x2013, constants::eeprom::camera_mode_offset, true);

        SFRField<uint32_t> images_written = SFRField<uint32_t>(0, 0x2014, constants::eeprom::camera_images_written_offset, true);
        SFRField<uint32_t> fragments_written = SFRField<uint32_t>(0, 0x2015, constants::eeprom::camera_fragments_written_offset, true);

        SFRField<uint32_t> set_res = SFRField<uint32_t>(VC0706_160x120, 0x2016, constants::eeprom::camera_set_res_offset, true);

        SFRField<uint16_t> failed_times = SFRField<uint16_t>(0, 0x2017, constants::eeprom::camera_failed_times_offset, true);
        SFRField<uint16_t> failed_limit = SFRField<uint16_t>(5, 0x2018, constants::eeprom::camera_failed_limit_offset, true);

        bool fragment_requested = false;
        SFRField<uint32_t> fragment_number_requested = SFRField<uint32_t>(0, 0x2019, constants::eeprom::camera_fragment_number_requested, true);
        SFRField<uint8_t> serial_requested = SFRField<uint8_t>(0, 0x2020, constants::eeprom::camera_serial_requested, true);

        bool report_written = false;
        bool report_ready = true;

    } // namespace camera
    namespace rockblock {
        // OP Codes 2100
        SFRField<bool> ready_status = SFRField<bool>(false, 0x2100, constants::eeprom::rockblock_ready_status_offset, true);

        SFRField<uint32_t> last_downlink = SFRField<uint32_t>(0, 0x2101, constants::eeprom::rockblock_last_downlink_offset, true);
        SFRField<uint32_t> downlink_period = SFRField<uint32_t>(0, 0x2102, constants::eeprom::rockblock_downlink_period_offset, true);

        SFRField<bool> waiting_message = SFRField<bool>(false, 0x2103, constants::eeprom::rockblock_waiting_message_offset, true);

        std::deque<uint8_t> downlink_report;
        std::deque<uint8_t> normal_report;
        std::deque<uint8_t> camera_report;
        std::deque<uint8_t> imu_report;

        uint8_t normal_report_command_curr = 0;
        uint8_t normal_report_command_max = constants::rockblock::normal_report_command_default_max;
        std::deque<uint8_t> commands_received;

        char buffer[constants::rockblock::buffer_size] = {0};
        int camera_commands[99][constants::rockblock::command_len] = {};
        uint32_t camera_max_fragments[99] = {};
        int commas[constants::rockblock::num_commas] = {0};

        std::deque<RawRockblockCommand> raw_commands;
        std::deque<RockblockCommand *> processed_commands;

        SFRField<uint8_t> max_commands_count = SFRField<uint8_t>(10, 0x2104, constants::eeprom::rockblock_max_commands_count_offset, true);

        SFRField<uint16_t> imu_max_fragments = SFRField<uint16_t>(256, 0x2105, constants::eeprom::rockblock_imu_max_fragments_offset, true);

        SFRField<uint32_t> imudownlink_start_time = SFRField<uint32_t>(0, 0x2106, constants::eeprom::rockblock_imudownlink_start_time_offset, true);
        SFRField<uint32_t> imudownlink_remain_time = SFRField<uint32_t>(constants::time::one_minute, 0x2107, constants::eeprom::rockblock_imudownlink_remain_time_offset, true);
        SFRField<bool> imu_first_start = SFRField<bool>(true, 0x2108, constants::eeprom::rockblock_imu_first_start_offset, true);
        SFRField<bool> imu_downlink_on = SFRField<bool>(true, 0x2109, constants::eeprom::rockblock_imu_downlink_on_offset, true);

        SFRField<bool> flush_status = SFRField<bool>(false, 0x2110, constants::eeprom::rockblock_flush_status_offset, true);
        SFRField<bool> waiting_command = SFRField<bool>(false, 0x2111, constants::eeprom::rockblock_waiting_command_offset, true);
        SFRField<uint32_t> conseq_reads = SFRField<uint32_t>(0, 0x2112, constants::eeprom::rockblock_conseq_reads_offset, true);
        SFRField<uint32_t> timeout = SFRField<uint32_t>(10 * constants::time::one_minute, 0x2113, constants::eeprom::rockblock_timeout_offset, true);
        SFRField<uint32_t> start_time = SFRField<uint32_t>(0, 0x2114, constants::eeprom::rockblock_start_time_offset, true);
        SFRField<uint32_t> start_time_check_signal = SFRField<uint32_t>(0, 0x2115, constants::eeprom::rockblock_start_time_check_signal_offset, true);
        SFRField<uint32_t> max_check_signal_time = SFRField<uint32_t>(constants::time::one_minute, 0x2116, constants::eeprom::rockblock_max_check_signal_time_offset, true);
        SFRField<bool> sleep_mode = SFRField<bool>(false, 0x2117, constants::eeprom::rockblock_sleep_mode_offset, true);

        SFRField<uint16_t> downlink_report_type = SFRField<uint16_t>((uint16_t)report_type::normal_report, 0x2118, constants::eeprom::rockblock_downlink_report_type_offset, true);
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)rockblock_mode_type::send_at, 0x2119, constants::eeprom::rockblock_mode_offset, true);

#ifndef SIMULATOR
        HardwareSerial serial = Serial1;
#else
        RockblockSimulator serial;
#endif
    } // namespace rockblock
    namespace imu {
        // OP Codes 2200
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)sensor_mode_type::init, 0x2200, constants::eeprom::imu_mode_offset, true);

        // change to init mode
        SFRField<uint16_t> init_mode = SFRField<uint16_t>((uint16_t)sensor_init_mode_type::awaiting, 0x2201, constants::eeprom::imu_init_mode_offset, true);

        SFRField<uint32_t> max_fragments = SFRField<uint32_t>(256, 0x2202, constants::eeprom::imu_max_fragments_offset, true);

        SFRField<bool> sample_gyro = SFRField<bool>(true, 0x2203, constants::eeprom::imu_sample_gyro_offset, true);

        SFRField<bool> turn_on = SFRField<bool>(false, 0x2204, constants::eeprom::imu_turn_on_offset, true);
        SFRField<bool> turn_off = SFRField<bool>(false, 0x2205, constants::eeprom::imu_turn_off_offset, true);
        SFRField<bool> powered = SFRField<bool>(false, 0x2206, constants::eeprom::imu_powered_offset, true);

        SFRField<uint16_t> failed_times = SFRField<uint16_t>(0, 0x2207, constants::eeprom::imu_failed_times_offset, true);
        SFRField<uint16_t> failed_limit = SFRField<uint16_t>(5, 0x2208, constants::eeprom::imu_failed_limit_offset, true);

        SFRField<uint16_t> imu_boot_collection_start_time = SFRField<uint16_t>(0, 0x2209, 0, true);
        SFRField<uint16_t> door_open__collection_start_time = SFRField<uint16_t>(0, 0x220a, 0, true);

        SensorReading *mag_x_value = new SensorReading(1, 0, 0);
        SensorReading *mag_y_value = new SensorReading(1, 0, 0);
        SensorReading *mag_z_value = new SensorReading(1, 0, 0);

        SensorReading *gyro_x_value = new SensorReading(1, 0, 0);
        SensorReading *gyro_y_value = new SensorReading(1, 0, 0);
        SensorReading *gyro_z_value = new SensorReading(1, 0, 0);

        SensorReading *mag_x_average = new SensorReading(fault_index_type::mag_x, 20, 0, 0);
        SensorReading *mag_y_average = new SensorReading(fault_index_type::mag_y, 20, 0, 0);
        SensorReading *mag_z_average = new SensorReading(fault_index_type::mag_z, 20, 0, 0);

        SensorReading *gyro_x_average = new SensorReading(fault_index_type::gyro_x, 20, 0, 0);
        SensorReading *gyro_y_average = new SensorReading(fault_index_type::gyro_y, 20, 0, 0);
        SensorReading *gyro_z_average = new SensorReading(fault_index_type::gyro_z, 20, 0, 0);

        SensorReading *acc_x_average = new SensorReading(fault_index_type::acc_x, 20, 0, 0);
        SensorReading *acc_y_average = new SensorReading(fault_index_type::acc_y, 20, 0, 0);

        std::deque<uint8_t> imu_dlink;

        boolean report_written = false;
        boolean report_ready = false;

    } // namespace imu
    namespace temperature {
        // OP Codes 2300
        SFRField<bool> in_sun = SFRField<bool>(false, 0x2300, constants::eeprom::temperature_in_sun_offset, true);

        SensorReading *temp_c_average = new SensorReading(fault_index_type::temp_c, 1500, -500, 500);
        SensorReading *temp_c_value = new SensorReading(1, -500, 500);
    } // namespace temperature
    namespace current {
        // OP Codes 2400
        SFRField<bool> in_sun = SFRField<bool>(false, 0x2400, constants::eeprom::current_in_sun_offset, true);

        SensorReading *solar_current_average = new SensorReading(fault_index_type::solar_current, 1500, 0, 1000);
    } // namespace current
    namespace acs {
        // OP Codes 2500
        SFRField<uint32_t> max_no_communication = SFRField<uint32_t>(0, 0x2500, constants::eeprom::acs_max_no_communication_offset, true);

#ifndef E2E_TESTING
        SFRField<uint32_t> on_time = SFRField<uint32_t>(5 * constants::time::one_minute, 0x2501, constants::eeprom::acs_on_time_offset, true);
#endif
        SFRField<bool> off = SFRField<bool>(true, 0x2502, constants::eeprom::acs_off_offset, true);

        SFRField<uint16_t> mag = SFRField<uint16_t>((uint16_t)simple_acs_type::x, 0x2503, constants::eeprom::acs_mag_offset, true);
        SFRField<uint32_t> detumble_timeout = SFRField<uint32_t>(30 * constants::time::one_second, 0x2504, constants::eeprom::acs_detumble_timeout_offset, true);
#ifdef E2E_TESTING
        SFRField<uint32_t> on_time = SFRField<uint32_t>(5 * constants::time::one_second, 0x2501, constants::eeprom::acs_on_time_offset, true);
#endif
    } // namespace acs
    namespace battery {
        // OP Codes 2600
        // TODO
        SFRField<uint32_t> acceptable_battery = SFRField<uint32_t>(3.75, 0x2600, constants::eeprom::battery_acceptable_battery_offset, true);
        SFRField<uint32_t> min_battery = SFRField<uint32_t>(0, 0x2601, constants::eeprom::battery_min_battery_offset, true);

        SensorReading *voltage_value = new SensorReading(1, 0, 5);
        SensorReading *voltage_average = new SensorReading(fault_index_type::voltage, 300, 0, 5);
    } // namespace battery
    namespace button {
        // OP Codes 2700
        SFRField<bool> pressed = SFRField<bool>(true, 0x2700, constants::eeprom::button_pressed_offset, true);
        SensorReading *button_pressed = new SensorReading(fault_index_type::button, 1, 1, 0);
    } // namespace button
    namespace pins {
        std::map<int, int> pinMap = {
            {constants::acs::xPWMpin, LOW},
            {constants::acs::yPWMpin, LOW},
            {constants::acs::zPWMpin, LOW},
            {constants::acs::yout1, LOW},
            {constants::acs::yout2, LOW},
            {constants::acs::xout1, LOW},
            {constants::acs::xout2, LOW},
            {constants::acs::zout1, LOW},
            {constants::acs::zout2, LOW},
            {constants::camera::power_on_pin, LOW},
            {constants::camera::rx, LOW},
            {constants::camera::tx, LOW},
            {constants::button::button_pin, HIGH},
            {constants::battery::voltage_value_pin, LOW},
            {constants::acs::STBXYpin, LOW},
            {constants::acs::STBZpin, LOW},
            {constants::photoresistor::pin, LOW},
            {constants::temperature::pin, LOW},
            {constants::current::pin, LOW},
            {constants::burnwire::first_pin, LOW},
            {constants::burnwire::second_pin, LOW},
            {constants::rockblock::sleep_pin, LOW}};

    } // namespace pins
    namespace eeprom {
        // OP Codes 2800
        SFRField<uint8_t> boot_counter = SFRField<uint8_t>(0, 0x2800, constants::eeprom::eeprom_boot_count_offset, true);

        // Tracking the initial 2 hour wait time
        int wait_time_last_write_time = 0;                                                                                                                 // the millis() value when the last EEPROM wait time write ocurred, should reset to 0 every boot up cycle
        SFRField<uint32_t> wait_time_write_step_time = SFRField<uint32_t>(1000, 0x2801, constants::eeprom::eeprom_wait_time_write_step_time_offset, true); // the amount of time between each write to EEPROM for wait time
        SFRField<uint32_t> alloted_time = SFRField<uint32_t>(7200000, 0x2802, constants::eeprom::eeprom_alloted_time_offset, true);                        // the amount of time for the EEPROM to count to (7200000 ms = 2 h)
        SFRField<bool> alloted_time_passed = SFRField<bool>(false, 0x2803, constants::eeprom::eeprom_alloted_time_passed_offset, true);                    // indicates if the EEPROM count has reached the alloted time

        // Saving SFR data
        int sfr_last_write_time = 0;                                                                                                           // the millis() value when the last EEPROM SFR data write ocurred, should reset every boot up cycle
        SFRField<uint32_t> sfr_write_step_time = SFRField<uint32_t>(1000, 0x2804, constants::eeprom::eeprom_sfr_write_step_time_offset, true); // the amount of time between each write to EEPROM for SFR data
        uint16_t sfr_address = 7;                                                                                                              // the address of where current SFR data is stored, read from EEPROM and set at the beginning of every boot up cycle
        SFRField<uint32_t> sfr_address_age = SFRField<uint32_t>(0, 0x2805, constants::eeprom::eeprom_sfr_address_age_offset, true);            // the write age of the current SFR data section in EEPROM
        // Bytes 0-3 are for the time passed, byte 4 holds the number of reboots, and bytes 5-6 are for the address where values are to be written and stored.
        // SFR data begins at byte 7 and after. The section of EEPROM bytes where SFR data is stored will change to avoid exceeding the write endurance.
        SFRField<boolean> storage_full = SFRField<boolean>(false, 0x2806, constants::eeprom::eeprom_storage_full_offset, true);
    } // namespace eeprom
};    // namespace sfr

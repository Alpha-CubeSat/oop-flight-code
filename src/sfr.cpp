#include "sfr.hpp"

namespace sfr {
    namespace stabilization {
        // OP Codes 1100
        SFRField<uint32_t> max_time(30 * constants::time::one_minute, 0UL, 5 * constants::time::one_hour, 1100);
    } // namespace stabilization
    namespace boot {
        // OP Codes 1200
        SFRField<uint32_t> max_time(2 * constants::time::one_hour, 10UL, 5 * constants::time::one_hour, 1200);
    } // namespace boot
    namespace simple {
        // OP Codes 1300
        SFRField<uint32_t> max_time(5 * constants::time::one_minute, 10UL, 5 * constants::time::one_hour, 1300);
    } // namespace simple
    namespace point {
        // OP Codes 1400
        SFRField<uint32_t> max_time(5 * constants::time::one_minute, 1400);
    } // namespace point
    namespace detumble {
        // OP Codes 1500
        SFRField<uint32_t> start_time(0UL, 1500);
        SFRField<uint32_t> max_time(2 * constants::time::one_hour, 1501);
        // TODO
        SFRField<uint16_t> num_imu_retries(0, 1502);
        SFRField<uint16_t> max_imu_retries(5, 1503);

        float min_stable_gyro_z = 1;
        float max_stable_gyro_x = 0.2;
        float max_stable_gyro_y = 0.2;

        float min_unstable_gyro_x = 0.7;
        float min_unstable_gyro_y = 0.7;
    } // namespace detumble
    namespace aliveSignal {
        // OP Codes 1600
        SFRField<uint16_t> max_downlink_hard_faults(3, 1600);
        SFRField<bool> downlinked(false, 1601);
        SFRField<uint32_t> max_time(2 * constants::time::one_hour, 1602);
        SFRField<uint16_t> num_hard_faults(0, 1603);
    } // namespace aliveSignal
    namespace pins {
        // OP Codes N/A
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
        // OP Codes 1700
        int val = 0;
        SFRField<bool> covered(true, 1700);
        std::deque<int> light_val_buffer;
        SensorReading *light_val_average = new SensorReading(fault_index_type::light_val, 0.0, false);
    } // namespace photoresistor
    namespace mission {
        // OP Codes 1800
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

        SFRField<uint32_t> acs_transmit_cycle_time(constants::time::one_minute * 100, 1800);

        SFRField<uint32_t> time_deployed(0, 1801);
        SFRField<bool> deployed(false, 1802);
        SFRField<bool> already_deployed(false, 1803);

    } // namespace mission
    namespace burnwire {
        // OP Codes 1900
        burnwire_mode_type mode = burnwire_mode_type::standby;
        SFRField<bool> fire(false, 1900);
        SFRField<bool> arm(false, 1901);
        SFRField<uint16_t> attempts(0, 1902);
        SFRField<uint16_t> camera_attempts(0, 1904);
        SFRField<uint32_t> start_time(0, 1903);
        SFRField<uint32_t> burn_time(500, 1905);
        SFRField<uint32_t> armed_time(2 * constants::time::one_day, 1906);
    } // namespace burnwire
    namespace camera {
        // OP Codes 2000
        sensor_mode_type mode = sensor_mode_type::normal;
        SFRField<bool> photo_taken_sd_failed(false, 2000);
        SFRField<bool> take_photo(false, 2001);
        SFRField<bool> turn_on(false, 2002);
        SFRField<bool> turn_off(false, 2003);
        SFRField<bool> powered(false, 2004);

        // Initialization
        camera_init_mode_type init_mode = camera_init_mode_type::awaiting;
        SFRField<uint8_t> start_progress(0, 2005);
        SFRField<uint32_t> step_time(0, 2006);
        SFRField<uint32_t> init_start_time(0, 2007);
        SFRField<uint32_t> init_timeout(12000, 2008);
        SFRField<uint32_t> begin_delay(100, 2009);
        SFRField<uint32_t> resolution_set_delay(500, 2010);
        SFRField<uint32_t> resolution_get_delay(200, 2011);

        //@Lauren What here should be settable
        int current_serial = 0;
        int fragment_number = 0;
        int fragment_number_requested = 3; //@Lauren why's this = 3?
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
        // OP Codes 2100
        report_type downlink_report_type = report_type::normal_report;
        rockblock_mode_type mode = rockblock_mode_type::send_at;
        SFRField<bool> rockblock_ready_status(false, 2100);

        SFRField<uint32_t> last_downlink(0, 2101);
        SFRField<uint32_t> downlink_period(0, 2102);

        SFRField<bool> waiting_message(false, 2103);

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
        SFRField<uint8_t> max_commands_count(10, 2104);

        int imu_downlink_max_fragments[99] = {};
        SFRField<uint16_t> imu_max_fragments(256, 2105);

        SFRField<uint32_t> imudownlink_start_time(0, 2106);
        SFRField<uint32_t> imudownlink_remain_time(constants::time::one_minute, 2107);
        SFRField<bool> imu_first_start(true, 2108);
        SFRField<bool> imu_downlink_on(true, 2109);
#ifndef SIMULATOR
        HardwareSerial serial = Serial1;
#else
        RockblockSimulator serial;
#endif
        SFRField<bool> flush_status(false, 2108);
        SFRField<bool> waiting_command(false, 2109);
        SFRField<uint32_t> conseq_reads(0, 2110);
        SFRField<uint32_t> timeout(10 * constants::time::one_minute, 2111);
        SFRField<uint32_t> start_time(0, 2112);
        SFRField<uint32_t> start_time_check_signal(0, 2113);
        SFRField<uint32_t> max_check_signal_time(constants::time::one_minute, 2114);
        SFRField<bool> sleep_mode(false, 2115);
    } // namespace rockblock
    namespace imu {
        // OP Codes 2200
        sensor_mode_type mode = sensor_mode_type::init;
        SFRField<bool> successful_init(true, 2200);

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

        //@Lauren Which of these should be settable
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
        // OP Codes 2300
        float temp_c = 0.0;
        std::deque<float> temp_c_buffer;
        SensorReading *temp_c_average = new SensorReading(fault_index_type::temp_c, 0.0, false);
        SFRField<bool> in_sun(false, 2300);
    } // namespace temperature
    namespace current {
        // OP Codes 2400
        float solar_current = 0.0;
        std::deque<float> solar_current_buffer;
        SensorReading *solar_current_average = new SensorReading(fault_index_type::solar_current, 0.0, false);
        SFRField<bool> in_sun(false, 2400);
    } // namespace current

    namespace acs {
        // OP Codes 2500
        float current1 = 0;
        float current2 = 0;
        float current3 = 0;
        float pwm1 = 0;
        float pwm2 = 0;
        float pwm3 = 0;
        simple_acs_type mag = simple_acs_type::x;
        SFRField<uint32_t> max_no_communication(0, 2500);

        SFRField<uint32_t> on_time(5 * constants::time::one_minute, 2501);
        SFRField<bool> off(true, 2502);
    } // namespace acs
    namespace battery {
        // OP Codes 2600
        float voltage = 0.0;
        std::deque<float> voltage_buffer;
        SensorReading *voltage_average = new SensorReading(fault_index_type::voltage, 0.0, false);
        // TODO
        float acceptable_battery;
        float min_battery;
    } // namespace battery
    namespace fault {
        // OP Codes 2700
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
        // OP Codes 2800
        SFRField<bool> pressed(true, 2800);
    } // namespace button
    namespace EEPROM {
        // OP Codes 2900
        SFRField<uint32_t> time_of_last_write(0, 2900);
        SFRField<uint32_t> write_step_time(1000, 2901); // the amount of time between each write to EEPROM
        SFRField<uint32_t> alloted_time(7200000, 2902); // the amount of time for the EEPROM to count to (7200000 ms = 2 h)
        SFRField<uint32_t> eeprom_value(0, 2905);       // the amount of time that the EEPROM has counted, stops when the alloted time has been reached
        SFRField<bool> alloted_time_passed(false, 2904);
    } // namespace EEPROM
} // namespace sfr
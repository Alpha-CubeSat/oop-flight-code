#include "sfr.hpp"

namespace sfr {
    namespace stabilization {
        // OP Codes 1100
        SFRField<uint32_t> max_time = SFRField<uint32_t>(30 * constants::time::one_minute, 0UL, 5 * constants::time::one_hour, 1100);
    } // namespace stabilization
    namespace boot {
        // OP Codes 1200
        SFRField<uint32_t> max_time = SFRField<uint32_t>(2 * constants::time::one_hour, 10UL, 5 * constants::time::one_hour, 1200);
    } // namespace boot
    namespace simple {
        // OP Codes 1300
        SFRField<uint32_t> max_time = SFRField<uint32_t>(5 * constants::time::one_minute, 10UL, 5 * constants::time::one_hour, 1300);
    } // namespace simple
    namespace point {
        // OP Codes 1400
        SFRField<uint32_t> max_time = SFRField<uint32_t>(5 * constants::time::one_minute, 1400);
    } // namespace point
    namespace detumble {
        // OP Codes 1500
        SFRField<uint32_t> start_time = SFRField<uint32_t>(0UL, 1500);
        SFRField<uint32_t> max_time = SFRField<uint32_t>(2 * constants::time::one_hour, 1501);
        // TODO
        SFRField<uint16_t> num_imu_retries = SFRField<uint16_t>(0, 1502);
        SFRField<uint16_t> max_imu_retries = SFRField<uint16_t>(5, 1503);

        SFRField<uint8_t> min_stable_gyro_z = SFRField<uint8_t>(1, 0, 2, 1504, 10);
        SFRField<uint8_t> max_stable_gyro_x = SFRField<uint8_t>(.2, 0, 1, 1505, 10);
        SFRField<uint8_t> max_stable_gyro_y = SFRField<uint8_t>(.2, 0, 1, 1506, 10);

        SFRField<uint8_t> min_unstable_gyro_x = SFRField<uint8_t>(.7, 0, 1, 1507, 10);
        SFRField<uint8_t> min_unstable_gyro_y = SFRField<uint8_t>(.7, 0, 1, 1508, 10);
    } // namespace detumble
    namespace aliveSignal {
        // OP Codes 1600
        SFRField<uint16_t> max_downlink_hard_faults = SFRField<uint16_t>(3, 1600);
        SFRField<bool> downlinked = SFRField<bool>(false, 1601);
        SFRField<uint32_t> max_time = SFRField<uint32_t>(2 * constants::time::one_hour, 1602);
        SFRField<uint16_t> num_hard_faults = SFRField<uint16_t>(0, 1603);
    } // namespace aliveSignal
    namespace pins {
        // TODO confirm initial pin states are correct FS-159
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
        SFRField<bool> covered = SFRField<bool>(true, 1700);

        SensorReading *light_val_average = new SensorReading(fault_index_type::light_val, 20, 0, 1000);
    } // namespace photoresistor
    namespace mission {
        // OP Codes 1800
        SFRField<uint32_t> acs_transmit_cycle_time = SFRField<uint32_t>(constants::time::one_minute * 100, 1800);

        SFRField<uint32_t> time_deployed = SFRField<uint32_t>(0, 1801);
        SFRField<bool> deployed = SFRField<bool>(false, 1802);
        SFRField<bool> already_deployed = SFRField<bool>(false, 1803);

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
    } // namespace mission
    namespace burnwire {
        // OP Codes 1900
        SFRField<bool> fire = SFRField<bool>(false, 1900);
        SFRField<bool> arm = SFRField<bool>(false, 1901);
        SFRField<uint16_t> attempts = SFRField<uint16_t>(0, 1902);
        SFRField<uint16_t> camera_attempts = SFRField<uint16_t>(0, 1904);
        SFRField<uint32_t> start_time = SFRField<uint32_t>(0, 1903);
        SFRField<uint32_t> burn_time = SFRField<uint32_t>(500, 1905);
        SFRField<uint32_t> armed_time = SFRField<uint32_t>(2 * constants::time::one_day, 1906);
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)burnwire_mode_type::standby, 1907);
        SFRField<uint16_t> attempts_limit = SFRField<uint16_t>(10, 1908);
    } // namespace burnwire
    namespace camera {
        // OP Codes 2000
        SFRField<bool> photo_taken_sd_failed = SFRField<bool>(false, 2000);
        SFRField<bool> take_photo = SFRField<bool>(false, 2001);
        SFRField<bool> turn_on = SFRField<bool>(false, 2002);
        SFRField<bool> turn_off = SFRField<bool>(false, 2003);
        SFRField<bool> powered = SFRField<bool>(false, 2004);

        // Initialization
        SFRField<uint8_t> start_progress = SFRField<uint8_t>(0, 2005);
        SFRField<uint32_t> step_time = SFRField<uint32_t>(0, 2006);
        SFRField<uint32_t> init_start_time = SFRField<uint32_t>(0, 2007);
        SFRField<uint32_t> init_timeout = SFRField<uint32_t>(12000, 2008);
        SFRField<uint32_t> begin_delay = SFRField<uint32_t>(100, 2009);
        SFRField<uint32_t> resolution_set_delay = SFRField<uint32_t>(500, 2010);
        SFRField<uint32_t> resolution_get_delay = SFRField<uint32_t>(200, 2011);

        SFRField<uint16_t> init_mode = SFRField<uint16_t>((uint16_t)camera_init_mode_type::awaiting, 2012);
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)sensor_mode_type::normal, 2013);

        SFRField<uint32_t> images_written = SFRField<uint32_t>(0, 2014);
        SFRField<uint32_t> fragments_written = SFRField<uint32_t>(0, 2015);

        SFRField<uint32_t> set_res = SFRField<uint32_t>(VC0706_160x120, 2016);

        SFRField<uint16_t> failed_times = SFRField<uint16_t>(0, 2017);
        SFRField<uint16_t> failed_limit = SFRField<uint16_t>(5, 2018);

        boolean report_written = false;
        boolean report_downlinked = true;
        boolean report_ready = true;

    } // namespace camera
    namespace rockblock {
        // OP Codes 2100
        SFRField<bool> rockblock_ready_status = SFRField<bool>(false, 2100);

        SFRField<uint32_t> last_downlink = SFRField<uint32_t>(0, 2101);
        SFRField<uint32_t> downlink_period = SFRField<uint32_t>(0, 2102);

        SFRField<bool> waiting_message = SFRField<bool>(false, 2103);

        std::deque<uint8_t> downlink_report;
        std::deque<uint8_t> normal_report;
        std::deque<uint8_t> camera_report;
        std::deque<uint8_t> imu_report;

        char buffer[constants::rockblock::buffer_size] = {0};
        int camera_commands[99][constants::rockblock::command_len] = {};
        uint32_t camera_max_fragments[99] = {};
        int commas[constants::rockblock::num_commas] = {0};

        std::deque<RawRockblockCommand> raw_commands;
        std::deque<RockblockCommand*> processed_commands;

        SFRField<uint8_t> max_commands_count = SFRField<uint8_t>(10, 2104);

        SFRField<uint16_t> imu_max_fragments = SFRField<uint16_t>(256, 2105);

        SFRField<uint32_t> imudownlink_start_time = SFRField<uint32_t>(0, 2106);
        SFRField<uint32_t> imudownlink_remain_time = SFRField<uint32_t>(constants::time::one_minute, 2107);
        SFRField<bool> imu_first_start = SFRField<bool>(true, 2108);
        SFRField<bool> imu_downlink_on = SFRField<bool>(true, 2109);

        SFRField<bool> flush_status = SFRField<bool>(false, 2108);
        SFRField<bool> waiting_command = SFRField<bool>(false, 2109);
        SFRField<uint32_t> conseq_reads = SFRField<uint32_t>(0, 2110);
        SFRField<uint32_t> timeout = SFRField<uint32_t>(10 * constants::time::one_minute, 2111);
        SFRField<uint32_t> start_time = SFRField<uint32_t>(0, 2112);
        SFRField<uint32_t> start_time_check_signal = SFRField<uint32_t>(0, 2113);
        SFRField<uint32_t> max_check_signal_time = SFRField<uint32_t>(constants::time::one_minute, 2114);
        SFRField<bool> sleep_mode = SFRField<bool>(false, 2115);

        SFRField<uint16_t> downlink_report_type = SFRField<uint16_t>((uint16_t)report_type::normal_report, 2116);
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)rockblock_mode_type::send_at, 2117);

        #ifndef SIMULATOR
            HardwareSerial serial = Serial1;
        #else
            RockblockSimulator serial;
        #endif

    } // namespace rockblock
    namespace imu {
        // OP Codes 2200
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)sensor_mode_type::init, 2200);
        SFRField<bool> successful_init = SFRField<bool>(true, 2201);

        SFRField<uint32_t> max_fragments = SFRField<uint32_t>(256, 2202);

        SFRField<bool> sample_gyro = SFRField<bool>(true, 2204);

        SFRField<bool> turn_on = SFRField<bool>(false, 2205);
        SFRField<bool> turn_off = SFRField<bool>(false, 2206);
        SFRField<bool> powered = SFRField<bool>(false, 2207);

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
        boolean report_downlinked = true;
        boolean report_ready = true;

    } // namespace imu
    namespace temperature {
        // OP Codes 2300
        SFRField<bool> in_sun = SFRField<bool>(false, 2300);

        SensorReading *temp_c_average = new SensorReading(fault_index_type::temp_c, 20, -500, 500);
        SensorReading *temp_c_value = new SensorReading(1, -500, 500);
    } // namespace temperature
    namespace current {
        // OP Codes 2400
        SFRField<bool> in_sun = SFRField<bool>(false, 2400);

        SensorReading *solar_current_average = new SensorReading(fault_index_type::solar_current, 20, 0, 1000);
    } // namespace current
    namespace acs {
        // OP Codes 2500
        SFRField<uint32_t> max_no_communication = SFRField<uint32_t>(0, 2500);

        SFRField<uint32_t> on_time = SFRField<uint32_t>(5 * constants::time::one_minute, 2501);
        SFRField<bool> off = SFRField<bool>(true, 2502);

        SFRField<uint16_t> mag = SFRField<uint16_t>((uint16_t)simple_acs_type::x, 2503);
    } // namespace acs
    namespace battery {
        // OP Codes 2600
        // TODO
        SFRField<uint32_t> acceptable_battery = SFRField<uint32_t>(0, 2600);
        SFRField<uint32_t> min_battery = SFRField<uint32_t>(0, 2601);

        SensorReading *voltage_value = new SensorReading(1, 0, 5);
        SensorReading *voltage_average = new SensorReading(fault_index_type::voltage, 20, 0, 5);
    } // namespace battery
    namespace button {
        // OP Codes 2700
        SFRField<bool> pressed = SFRField<bool>(true, 2700);
    } // namespace button
    namespace EEPROM {
        // OP Codes 2800
        SFRField<uint32_t> time_of_last_write = SFRField<uint32_t>(0, 2800);
        SFRField<uint32_t> write_step_time = SFRField<uint32_t>(1000, 2801); // the amount of time between each write to EEPROM
        SFRField<uint32_t> alloted_time = SFRField<uint32_t>(7200000, 2802); // the amount of time for the EEPROM to count to (7200000 ms = 2 h)
        SFRField<uint32_t> eeprom_value = SFRField<uint32_t>(0, 2803);       // the amount of time that the EEPROM has counted, stops when the alloted time has been reached
        SFRField<bool> alloted_time_passed = SFRField<bool>(false, 2804);
    } // namespace EEPROM
};    // namespace sfr
#include "sfr.hpp"

namespace sfr {
    namespace stabilization {
        // OP Codes 1100
        // TODO actual default value
        SFRField<uint32_t> max_time = SFRField<uint32_t>(2 * constants::time::one_hour, 0x1100);
    } // namespace stabilization
    namespace boot {
        // OP Codes 1200
        SFRField<uint32_t> max_time = SFRField<uint32_t>(2 * constants::time::one_hour, 0x1200);
    } // namespace boot
    namespace detumble {
        // OP Codes 1500
        SFRField<uint8_t> min_stable_gyro_z = SFRField<uint8_t>((0.8 * constants::acs::target_spin_rate * constants::imu::sfr_resolution), 0x1500, constants::imu::sfr_resolution); // rad/s
        SFRField<uint8_t> max_stable_gyro_x = SFRField<uint8_t>((0.1 * constants::imu::sfr_resolution), 0x1501, constants::imu::sfr_resolution);                                    // rad/s
        SFRField<uint8_t> max_stable_gyro_y = SFRField<uint8_t>((0.1 * constants::imu::sfr_resolution), 0x1502, constants::imu::sfr_resolution);                                    // rad/s
        SFRField<uint8_t> min_unstable_gyro_x = SFRField<uint8_t>((0.7 * constants::imu::sfr_resolution), 0x1503, constants::imu::sfr_resolution);                                  // rad/s
        SFRField<uint8_t> min_unstable_gyro_y = SFRField<uint8_t>((0.7 * constants::imu::sfr_resolution), 0x1504, constants::imu::sfr_resolution);                                  // rad/s
    }                                                                                                                                                                               // namespace detumble
    namespace aliveSignal {
        // OP Codes 1600
        SFRField<bool> downlinked = SFRField<bool>(false, 0x1600);
        SFRField<uint16_t> max_downlink_hard_faults = SFRField<uint16_t>(3, 0x1601);
        SFRField<uint16_t> num_hard_faults = SFRField<uint16_t>(0, 0x1602);
        SFRField<uint32_t> max_time = SFRField<uint32_t>(2 * constants::time::one_hour, 0x1603);
    } // namespace aliveSignal
    namespace photoresistor {
        // OP Codes 1700
        SFRField<bool> covered = SFRField<bool>(true, 0x1700);

        SensorReading *light_val_average_standby = new SensorReading(fault_groups::hardware_faults::light_val, 20, constants::photoresistor::min_light_val, constants::photoresistor::max_light_val);
        SensorReading *light_val_average_deployment = new SensorReading(fault_groups::hardware_faults::light_val, 1, constants::photoresistor::min_light_val, constants::photoresistor::max_light_val);
    } // namespace photoresistor
    namespace mission {
        // OP Codes 1800
        SFRField<bool> deployed = SFRField<bool>(false, 0x1800);
        SFRField<bool> possible_uncovered = SFRField<bool>(false, 0x1801);
        SFRField<uint8_t> boot_time_mins = SFRField<uint8_t>(0, 0x1802);
        SFRField<uint32_t> cycle_no = SFRField<uint32_t>(0, 0x1803);
        SFRField<uint32_t> cycle_start = SFRField<uint32_t>(0, 0x1804);

        Boot boot_class;
        AliveSignal aliveSignal_class;
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
        CaptureIMU captureIMU_class;
        BootSensors bootSensors_class;
        MandatoryBurns mandatoryBurns_class;
        RegularBurns regularBurns_class;
        DeploymentVerification deploymentVerification_class;

        Initialization initialization_class;
        Stabilization stabilization_class;
        Standby standby_class;
        Deployment deployment_class;
        Armed armed_class;
        InSun insun_class;
        Firing firing_class;

        MissionMode *boot = &boot_class;
        MissionMode *aliveSignal = &aliveSignal_class;
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
        MissionMode *captureIMU = &captureIMU_class;
        MissionMode *bootSensors = &bootSensors_class;
        MissionMode *mandatoryBurns = &mandatoryBurns_class;
        MissionMode *regularBurns = &regularBurns_class;
        MissionMode *deploymentVerification = &deploymentVerification_class;

        Phase *initialization = &initialization_class;
        Phase *stabilization = &stabilization_class;
        Phase *standby = &standby_class;
        Phase *deployment = &deployment_class;
        Phase *armed = &armed_class;
        Phase *inSun = &insun_class;
        Phase *firing = &firing_class;

        MissionMode *current_mode = boot;
        MissionMode *previous_mode = boot;

        std::deque<int> mode_history;
    } // namespace mission
    namespace burnwire {
        // OP Codes 1900
        SFRField<uint16_t> attempts = SFRField<uint16_t>(0, 0x1900);
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)burnwire_mode_type::standby, 0x1901);
        SFRField<uint16_t> attempts_limit = SFRField<uint16_t>(10, 0x1902);
        SFRField<uint16_t> mandatory_attempts_limit = SFRField<uint16_t>(2, 0x1903);
        SFRField<uint32_t> start_time = SFRField<uint32_t>(0, 0x1904);
        SFRField<uint32_t> burn_time = SFRField<uint32_t>(600, 0, 5 * constants::time::one_second, 0x1905);
        SFRField<uint32_t> armed_time = SFRField<uint32_t>(48 * constants::time::one_hour, 0, 12 * constants::time::one_hour, 0x1906);
        SFRField<uint32_t> delay_time = SFRField<uint32_t>(constants::time::one_second, 0x1907);
    } // namespace burnwire
    namespace camera {
        // OP Codes 2000
        SFRField<bool> take_photo = SFRField<bool>(false, 0x2000);
        SFRField<bool> powered = SFRField<bool>(false, 0x2001);
        SFRField<bool> report_ready = SFRField<bool>(false, 0x2002);
        SFRField<bool> fragment_requested = SFRField<bool>(false, 0x2003);
        SFRField<uint8_t> start_progress = SFRField<uint8_t>(0, 0x2004);
        SFRField<uint8_t> serial_requested = SFRField<uint8_t>(0, 0x2005);
        SFRField<uint8_t> power_setting = SFRField<uint8_t>((uint8_t)sensor_power_mode_type::do_nothing, 0x2006);
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)sensor_mode_type::normal, 0x2007);
        SFRField<uint16_t> failed_times = SFRField<uint16_t>(0, 0x2008);
        SFRField<uint16_t> failed_limit = SFRField<uint16_t>(5, 0x2009);
        SFRField<uint16_t> init_mode = SFRField<uint16_t>((uint16_t)sensor_init_mode_type::awaiting, 0x2010);
        SFRField<uint32_t> init_start_time = SFRField<uint32_t>(0, 0x2011);
        SFRField<uint32_t> init_timeout = SFRField<uint32_t>(12 * constants::time::one_second, 0x2012);
        SFRField<uint32_t> images_written = SFRField<uint32_t>(0, 0x2013);
        SFRField<uint32_t> fragments_written = SFRField<uint32_t>(0, 0x2014);
        SFRField<uint32_t> set_res = SFRField<uint32_t>(VC0706_160x120, 0x2015);
        SFRField<uint32_t> fragment_number_requested = SFRField<uint32_t>(0, 0x2016);
    } // namespace camera
    namespace rockblock {
        // OP Codes 2100
        SFRField<bool> ready_status = SFRField<bool>(false, 0x2100);
        SFRField<bool> waiting_command = SFRField<bool>(false, 0x2101);
        SFRField<bool> sleep_mode = SFRField<bool>(false, 0x2102);
        SFRField<uint8_t> max_commands_count = SFRField<uint8_t>(6, 0x2103);
        SFRField<uint8_t> queue_limit = SFRField<uint8_t>(5, 0x2104);
        SFRField<uint16_t> downlink_report_type = SFRField<uint16_t>((uint16_t)report_type::normal_report, 0x2105);
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)rockblock_mode_type::standby, 0x2106);
        SFRField<uint32_t> last_downlink = SFRField<uint32_t>(0, 0x2107);
        SFRField<uint32_t> downlink_period = SFRField<uint32_t>(20 * constants::time::one_minute, 0, 2 * constants::time::one_day, 0x2108);
        SFRField<uint32_t> lp_downlink_period = SFRField<uint32_t>(constants::time::one_hour, constants::time::one_second, 2 * constants::time::one_day, 0x2109);
        SFRField<uint32_t> transmit_downlink_period = SFRField<uint32_t>(20 * constants::time::one_minute, constants::time::one_second, 2 * constants::time::one_day, 0x2110);
        SFRField<uint32_t> on_time = SFRField<uint32_t>(30 * constants::time::one_minute, 0, constants::time::one_revolution, 0x2111);

        char buffer[constants::rockblock::buffer_size] = {0};
        uint8_t commas[constants::rockblock::num_commas] = {0};
        uint32_t camera_max_fragments[99] = {0};
        std::deque<uint16_t> commands_received;
        std::deque<RockblockCommand *> processed_commands;
        std::deque<uint8_t> downlink_report;
        std::deque<uint8_t> normal_report;
        std::deque<uint8_t> camera_report;
        std::deque<uint8_t> imu_report;

#ifndef SIMULATOR
        HardwareSerial serial = Serial1;
#else
        RockblockSimulator serial;
#endif
    } // namespace rockblock
    namespace imu {
        // OP Codes 2200
        SFRField<bool> powered = SFRField<bool>(false, 0x2200);
        SFRField<bool> dlink_written = SFRField<bool>(false, 0x2201);
        SFRField<bool> report_ready = SFRField<bool>(false, 0x2202);
        SFRField<uint8_t> power_setting = SFRField<uint8_t>((uint8_t)sensor_power_mode_type::on, 0x2203);
        SFRField<uint8_t> max_fragments = SFRField<uint8_t>(20, 0x2204);
        SFRField<uint16_t> mode = SFRField<uint16_t>((uint16_t)sensor_mode_type::init, 0x2205);
        SFRField<uint16_t> init_mode = SFRField<uint16_t>((uint16_t)sensor_init_mode_type::awaiting, 0x2206);
        SFRField<uint16_t> failed_times = SFRField<uint16_t>(0, 0x2207);
        SFRField<uint16_t> failed_limit = SFRField<uint16_t>(5, 0x2208);

        SFRField<bool> imu_valid = SFRField<bool>(true, 0x2209);

        SensorReading *mag_x_value = new SensorReading(fault_groups::imu_faults::mag_x_value, 1, constants::imu::min_mag, constants::imu::max_mag);
        SensorReading *mag_y_value = new SensorReading(fault_groups::imu_faults::mag_y_value, 1, constants::imu::min_mag, constants::imu::max_mag);
        SensorReading *mag_z_value = new SensorReading(fault_groups::imu_faults::mag_z_value, 1, constants::imu::min_mag, constants::imu::max_mag);
        SensorReading *gyro_x_value = new SensorReading(fault_groups::imu_faults::gyro_x_value, 1, constants::imu::min_gyro, constants::imu::max_gyro);
        SensorReading *gyro_y_value = new SensorReading(fault_groups::imu_faults::gyro_y_value, 1, constants::imu::min_gyro, constants::imu::max_gyro);
        SensorReading *gyro_z_value = new SensorReading(fault_groups::imu_faults::gyro_z_value, 1, constants::imu::min_gyro, constants::imu::max_gyro);
        SensorReading *mag_x_average = new SensorReading(fault_groups::imu_faults::mag_x_average, 20, constants::imu::min_mag, constants::imu::max_mag);
        SensorReading *mag_y_average = new SensorReading(fault_groups::imu_faults::mag_y_average, 20, constants::imu::min_mag, constants::imu::max_mag);
        SensorReading *mag_z_average = new SensorReading(fault_groups::imu_faults::mag_z_average, 20, constants::imu::min_mag, constants::imu::max_mag);
        SensorReading *gyro_x_average = new SensorReading(fault_groups::imu_faults::gyro_x_average, 20, constants::imu::min_gyro, constants::imu::max_gyro);
        SensorReading *gyro_y_average = new SensorReading(fault_groups::imu_faults::gyro_y_average, 20, constants::imu::min_gyro, constants::imu::max_gyro);
        SensorReading *gyro_z_average = new SensorReading(fault_groups::imu_faults::gyro_z_average, 20, constants::imu::min_gyro, constants::imu::max_gyro);

        std::deque<uint8_t> imu_dlink;
    } // namespace imu
    namespace temperature {
        // OP Codes 2300
        SFRField<bool> in_sun = SFRField<bool>(false, 0x2300);

        SensorReading *temp_c_average = new SensorReading(fault_groups::power_faults::temp_c_average, 1500, constants::temperature::min_temp_c, constants::temperature::max_temp_c);
        SensorReading *temp_c_value = new SensorReading(fault_groups::power_faults::temp_c_value, 1, constants::temperature::min_temp_c, constants::temperature::max_temp_c);
    } // namespace temperature
    namespace current {
        // OP Codes 2400
        SFRField<bool> in_sun = SFRField<bool>(false, 0x2400);

        SensorReading *solar_current_average = new SensorReading(fault_groups::power_faults::solar_current_average, 1500, -75, 500);
    } // namespace current
    namespace acs {
        // OP Codes 2500
        SFRField<bool> off = SFRField<bool>(true, 0x2500);
        SFRField<uint8_t> mode = SFRField<uint8_t>((uint8_t)acs_mode_type::detumble, 0x2501);
        SFRField<uint8_t> simple_mag = SFRField<uint8_t>((uint8_t)mag_type::z, 0x2502);
        SFRField<uint32_t> simple_current = SFRField<uint32_t>(0, 0x2503);
        SFRField<uint32_t> on_time = SFRField<uint32_t>(10 * constants::time::one_minute, 0, constants::time::one_revolution, 0x2504);
        SFRField<uint32_t> Id_index = SFRField<uint32_t>(0, 0, (sizeof(constants::acs::Id_values) / sizeof(constants::acs::Id_values[0])) - 1, 0x2505);
        SFRField<uint32_t> Kd_index = SFRField<uint32_t>(0, 0, (sizeof(constants::acs::Kd_values) / sizeof(constants::acs::Kd_values[0])) - 1, 0x2506);
        SFRField<uint32_t> Kp_index = SFRField<uint32_t>(0, 0, (sizeof(constants::acs::Kp_values) / sizeof(constants::acs::Kp_values[0])) - 1, 0x2507);
        SFRField<uint32_t> c_index = SFRField<uint32_t>(0, 0, (sizeof(constants::acs::c_values) / sizeof(constants::acs::c_values[0])) - 1, 0x2508);

        SFRField<float> current_x = SFRField<float>(0, -0.3, 0.3, 0x2509, 1.0);
        SFRField<float> current_y = SFRField<float>(0, -0.3, 0.3, 0x2510, 1.0);
        SFRField<float> current_z = SFRField<float>(0, -0.3, 0.3, 0x2511, 1.0);

        SFRField<int32_t> pwm_x = SFRField<int32_t>(0, -256, 256, 0x2512);
        SFRField<int32_t> pwm_y = SFRField<int32_t>(0, -256, 256, 0x2513);
        SFRField<int32_t> pwm_z = SFRField<int32_t>(0, -256, 256, 0x2514);

    } // namespace acs
    namespace battery {
        // OP Codes 2600
        SFRField<uint32_t> acceptable_battery = SFRField<uint32_t>((3.9 * constants::battery::sfr_resolution), 0x2600, constants::battery::sfr_resolution);
        SFRField<uint32_t> min_battery = SFRField<uint32_t>((3.75 * constants::battery::sfr_resolution), 0x2601, constants::battery::sfr_resolution);

        SensorReading *voltage_value = new SensorReading(fault_groups::power_faults::voltage_value, 1, constants::battery::min_voltage, constants::battery::max_voltage);
        SensorReading *voltage_average = new SensorReading(fault_groups::power_faults::voltage_average, 300, constants::battery::min_voltage, constants::battery::max_voltage);
    } // namespace battery
    namespace button {
        // OP Codes 2700
        SFRField<bool> pressed = SFRField<bool>(true, 0x2700);

        SensorReading *button_pressed = new SensorReading(fault_groups::hardware_faults::button, 1, 0, 1);
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
            {constants::acs::STBXYpin, HIGH},
            {constants::acs::STBZpin, HIGH},
            {constants::burnwire::first_pin, LOW},
            {constants::burnwire::second_pin, LOW},
            {constants::rockblock::sleep_pin, LOW}};
    } // namespace pins
    namespace eeprom {
        // OP Codes 2800
        SFRField<bool> boot_mode = SFRField<bool>(true, 0x2800);
        SFRField<bool> boot_restarted = SFRField<bool>(false, 0x2801);
        SFRField<bool> error_mode = SFRField<bool>(false, 0x2802);
        SFRField<bool> light_switch = SFRField<bool>(false, 0x2803);
        SFRField<bool> sfr_save_completed = SFRField<bool>(false, 0x2804);
        SFRField<uint8_t> boot_counter = SFRField<uint8_t>(0, 0x2805);
        SFRField<uint16_t> dynamic_data_addr = SFRField<uint16_t>(constants::eeprom::dynamic_data_start, constants::eeprom::dynamic_data_start, constants::eeprom::sfr_data_start - 1, 0x2806);
        SFRField<uint16_t> sfr_data_addr = SFRField<uint16_t>(constants::eeprom::sfr_data_start, constants::eeprom::sfr_data_start, constants::eeprom::boot_time_loc2 - 1, 0x2807);
        SFRField<uint32_t> time_alive = SFRField<uint32_t>(0, 0x2808);
        SFRField<uint32_t> dynamic_data_age = SFRField<uint32_t>(0, 0x2809);
        SFRField<uint32_t> sfr_data_age = SFRField<uint32_t>(0, 0x2810);
    } // namespace eeprom
};    // namespace sfr
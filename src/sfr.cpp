#include "sfr.hpp"

namespace sfr {
namespace stabilization {
// OP Codes 1100
SFRField<uint32_t> max_time = SFRField<uint32_t>(
    constants::time::one_minute, 0UL, 5 * constants::time::one_hour, 0x1100);
}  // namespace stabilization
namespace boot {
// OP Codes 1200
SFRField<uint32_t> max_time =
    SFRField<uint32_t>(30000, 10UL, 5 * constants::time::one_hour, 0x1200);
}  // namespace boot
namespace detumble {
// OP Codes 1500
SFRField<uint8_t> min_stable_gyro_z = SFRField<uint8_t>(1, 0, 2, 0x1500, 10);
SFRField<uint8_t> max_stable_gyro_x = SFRField<uint8_t>(.2, 0, 1, 0x1501, 10);
SFRField<uint8_t> max_stable_gyro_y = SFRField<uint8_t>(.2, 0, 1, 0x1502, 10);
SFRField<uint8_t> min_unstable_gyro_x = SFRField<uint8_t>(.7, 0, 1, 0x1503, 10);
SFRField<uint8_t> min_unstable_gyro_y = SFRField<uint8_t>(.7, 0, 1, 0x1504);
}  // namespace detumble
namespace aliveSignal {
// OP Codes 1600
SFRField<bool> downlinked = SFRField<bool>(false, 0x1600);
SFRField<uint16_t> max_downlink_hard_faults = SFRField<uint16_t>(3, 0x1601);
SFRField<uint16_t> num_hard_faults = SFRField<uint16_t>(0, 0x1602);
SFRField<uint32_t> max_time = SFRField<uint32_t>(30000, 0x1603);
}  // namespace aliveSignal
namespace photoresistor {
// OP Codes 1700
SFRField<bool> covered = SFRField<bool>(true, 0x1700);

SensorReading *light_val_average_standby =
    new SensorReading(fault_groups::hardware_faults::light_val, 20,
                      constants::photoresistor::min_light_val,
                      constants::photoresistor::max_light_val);
SensorReading *light_val_average_deployment =
    new SensorReading(fault_groups::hardware_faults::light_val, 1,
                      constants::photoresistor::min_light_val,
                      constants::photoresistor::max_light_val);
}  // namespace photoresistor
namespace mission {
// OP Codes 1800
SFRField<bool> deployed = SFRField<bool>(false, 0x1800);
SFRField<bool> possible_uncovered = SFRField<bool>(false, 0x1801);
SFRField<uint32_t> mission_mode_hist_length = SFRField<uint32_t>(16, 0x1802);
SFRField<uint32_t> cycle_no = SFRField<uint32_t>(0, 0x1803);
SFRField<uint32_t> cycle_dur = SFRField<uint32_t>(0, 0x1804);
SFRField<uint32_t> mission_time = SFRField<uint32_t>(0, 0x1805);
SFRField<uint32_t> boot_time = SFRField<uint32_t>(0, 0x1805);

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
}  // namespace mission
namespace burnwire {
// OP Codes 1900
SFRField<uint16_t> attempts = SFRField<uint16_t>(0, 0, 10, 0x1900);
SFRField<uint16_t> mode =
    SFRField<uint16_t>((uint16_t)burnwire_mode_type::standby, 0x1901);
SFRField<uint16_t> attempts_limit = SFRField<uint16_t>(10, 0x1902);
SFRField<uint16_t> mandatory_attempts_limit = SFRField<uint16_t>(4, 0x1903);
SFRField<uint32_t> start_time = SFRField<uint32_t>(0, 0x1904);
SFRField<uint32_t> burn_time = SFRField<uint32_t>(600, 0, 5000, 0x1905);
SFRField<uint32_t> armed_time =
    SFRField<uint32_t>(2 * constants::time::one_day, 0, 864000000, 0x1906);
SFRField<uint32_t> delay_time = SFRField<uint32_t>(1000, 0x1907);
}  // namespace burnwire
namespace camera {
// OP Codes 2000
SFRField<bool> photo_taken_sd_failed = SFRField<bool>(false, 0x2000);
SFRField<bool> take_photo = SFRField<bool>(false, 0x2001);
SFRField<bool> turn_on = SFRField<bool>(false, 0x2002);
SFRField<bool> turn_off = SFRField<bool>(false, 0x2003);
SFRField<bool> powered = SFRField<bool>(false, 0x2004);
SFRField<bool> report_ready = SFRField<bool>(true, 0x2005);
SFRField<bool> fragment_requested = SFRField<bool>(false, 0x2006);
SFRField<uint8_t> start_progress = SFRField<uint8_t>(0, 0x2007);
SFRField<uint8_t> serial_requested = SFRField<uint8_t>(0, 0x2008);
SFRField<uint16_t> mode =
    SFRField<uint16_t>((uint16_t)sensor_mode_type::normal, 0x2009);
SFRField<uint16_t> failed_times = SFRField<uint16_t>(0, 0x2010);
SFRField<uint16_t> failed_limit = SFRField<uint16_t>(5, 0x2011);
SFRField<uint16_t> init_mode =
    SFRField<uint16_t>((uint16_t)sensor_init_mode_type::awaiting, 0x2012);
SFRField<uint32_t> step_time = SFRField<uint32_t>(0, 0x2013);
SFRField<uint32_t> init_start_time = SFRField<uint32_t>(0, 0x2014);
SFRField<uint32_t> init_timeout = SFRField<uint32_t>(12000, 0x2015);
SFRField<uint32_t> resolution_set_delay = SFRField<uint32_t>(500, 0x2016);
SFRField<uint32_t> resolution_get_delay = SFRField<uint32_t>(200, 0x2017);
SFRField<uint32_t> images_written = SFRField<uint32_t>(0, 0x2018);
SFRField<uint32_t> fragments_written = SFRField<uint32_t>(0, 0x2019);
SFRField<uint32_t> set_res = SFRField<uint32_t>(VC0706_160x120, 0x2020);
SFRField<uint32_t> fragment_number_requested = SFRField<uint32_t>(0, 0x2021);
}  // namespace camera
namespace rockblock {
// OP Codes 2100
SFRField<bool> ready_status = SFRField<bool>(false, 0x2100);
SFRField<bool> waiting_message = SFRField<bool>(false, 0x2101);
SFRField<bool> waiting_command = SFRField<bool>(false, 0x2102);
SFRField<bool> flush_status = SFRField<bool>(false, 0x2103);
SFRField<bool> sleep_mode = SFRField<bool>(false, 0x2104);
SFRField<uint8_t> max_commands_count = SFRField<uint8_t>(10, 0x2105);
SFRField<uint8_t> normal_report_command_curr = SFRField<uint8_t>(0, 0x2106);
SFRField<uint8_t> normal_report_command_max = SFRField<uint8_t>(
    constants::rockblock::normal_report_command_default_max, 0x2107);
SFRField<uint16_t> imu_max_fragments = SFRField<uint16_t>(256, 0x2108);
SFRField<uint16_t> downlink_report_type =
    SFRField<uint16_t>((uint16_t)report_type::normal_report, 0x2109);
SFRField<uint16_t> mode =
    SFRField<uint16_t>((uint16_t)rockblock_mode_type::send_at, 0x2110);
SFRField<uint32_t> last_downlink = SFRField<uint32_t>(0, 0x2111);
SFRField<uint32_t> downlink_period =
    SFRField<uint32_t>(0, 60000, 172800000, 0x2112);
SFRField<uint32_t> conseq_reads = SFRField<uint32_t>(0, 0x2113);
SFRField<uint32_t> start_time_check_signal = SFRField<uint32_t>(0, 0x2114);
SFRField<uint32_t> max_check_signal_time =
    SFRField<uint32_t>(constants::time::one_minute, 0x2115);
SFRField<uint32_t> on_time =
    SFRField<uint32_t>(constants::time::one_minute, 0x2116);

char buffer[constants::rockblock::buffer_size] = {0};
int camera_commands[99][constants::rockblock::command_len] = {0};
int commas[constants::rockblock::num_commas] = {0};
uint32_t camera_max_fragments[99] = {0};
std::deque<RawRockblockCommand> raw_commands;
std::deque<uint8_t> commands_received;
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
}  // namespace rockblock
namespace imu {
// OP Codes 2200
SFRField<bool> sample_gyro = SFRField<bool>(true, 0x2200);
SFRField<bool> turn_on = SFRField<bool>(false, 0x2201);
SFRField<bool> turn_off = SFRField<bool>(false, 0x2202);
SFRField<bool> powered = SFRField<bool>(false, 0x2203);
SFRField<bool> report_written = SFRField<bool>(false, 0x2204);
SFRField<bool> report_ready = SFRField<bool>(false, 0x2205);
SFRField<uint16_t> mode =
    SFRField<uint16_t>((uint16_t)sensor_mode_type::init, 0x2206);
SFRField<uint16_t> init_mode =
    SFRField<uint16_t>((uint16_t)sensor_init_mode_type::awaiting, 0x2207);
SFRField<uint16_t> failed_times = SFRField<uint16_t>(0, 0x2208);
SFRField<uint16_t> failed_limit = SFRField<uint16_t>(5, 0x2209);
SFRField<uint16_t> imu_boot_collection_start_time =
    SFRField<uint16_t>(0, 0x2210);
SFRField<uint16_t> door_open__collection_start_time =
    SFRField<uint16_t>(0, 0x2211);
SFRField<uint32_t> max_fragments = SFRField<uint32_t>(256, 0x2212);

SensorReading *mag_x_value =
    new SensorReading(fault_groups::imu_faults::mag_x_value, 1, -100, 100);
SensorReading *mag_y_value =
    new SensorReading(fault_groups::imu_faults::mag_y_value, 1, -100, 100);
SensorReading *mag_z_value =
    new SensorReading(fault_groups::imu_faults::mag_z_value, 1, -100, 100);
SensorReading *gyro_x_value =
    new SensorReading(fault_groups::imu_faults::gyro_x_value, 1, -100, 100);
SensorReading *gyro_y_value =
    new SensorReading(fault_groups::imu_faults::gyro_y_value, 1, -100, 100);
SensorReading *gyro_z_value =
    new SensorReading(fault_groups::imu_faults::gyro_z_value, 1, -100, 100);
SensorReading *mag_x_average =
    new SensorReading(fault_groups::imu_faults::mag_x_average, 20, 0, 0);
SensorReading *mag_y_average =
    new SensorReading(fault_groups::imu_faults::mag_y_average, 20, 0, 0);
SensorReading *mag_z_average =
    new SensorReading(fault_groups::imu_faults::mag_z_average, 20, 0, 0);
SensorReading *gyro_x_average =
    new SensorReading(fault_groups::imu_faults::gyro_x_average, 20, 0, 0);
SensorReading *gyro_y_average =
    new SensorReading(fault_groups::imu_faults::gyro_y_average, 20, 0, 0);
SensorReading *gyro_z_average =
    new SensorReading(fault_groups::imu_faults::gyro_z_average, 20, 0, 0);
SensorReading *acc_x_average =
    new SensorReading(fault_groups::imu_faults::acc_x_average, 20, 0, 0);
SensorReading *acc_y_average =
    new SensorReading(fault_groups::imu_faults::acc_y_average, 20, 0, 0);

bool report_written = false;
bool report_ready = false;
std::deque<uint8_t> imu_dlink;
}  // namespace imu
namespace temperature {
// OP Codes 2300
SFRField<bool> in_sun = SFRField<bool>(false, 0x2300);

SensorReading *temp_c_average = new SensorReading(
    fault_groups::power_faults::temp_c_average, 1500, -100, 200);
SensorReading *temp_c_value =
    new SensorReading(fault_groups::power_faults::temp_c_value, 1, -500, 500);
}  // namespace temperature
namespace current {
// OP Codes 2400
SFRField<bool> in_sun = SFRField<bool>(false, 0x2400);

SensorReading *solar_current_average = new SensorReading(
    fault_groups::power_faults::solar_current_average, 1500, -75, 500);
}  // namespace current
namespace acs {
// OP Codes 2500
SFRField<bool> off = SFRField<bool>(false, 0x2500);
SFRField<uint8_t> mode =
    SFRField<uint8_t>((uint8_t)acs_mode_type::detumble, 0x2501);
SFRField<uint8_t> simple_mag = SFRField<uint8_t>(false, 0x2502);
SFRField<uint32_t> simple_current = SFRField<uint32_t>(false, 0x2503);
SFRField<uint32_t> on_time =
    SFRField<uint32_t>(constants::time::one_minute, 0x2504);
SFRField<uint32_t> Id_index = SFRField<uint32_t>(
    0, 0,
    (sizeof(constants::acs::Id_values) / sizeof(constants::acs::Id_values[0])) -
        1,
    0x2505);
SFRField<uint32_t> Kd_index = SFRField<uint32_t>(
    0, 0,
    (sizeof(constants::acs::Kd_values) / sizeof(constants::acs::Kd_values[0])) -
        1,
    0x2506);
SFRField<uint32_t> Kp_index = SFRField<uint32_t>(
    0, 0,
    (sizeof(constants::acs::Kp_values) / sizeof(constants::acs::Kp_values[0])) -
        1,
    0x2507);
SFRField<uint32_t> c_index = SFRField<uint32_t>(
    0, 0,
    (sizeof(constants::acs::c_values) / sizeof(constants::acs::c_values[0])) -
        1,
    0x2508);
}  // namespace acs
namespace battery {
// OP Codes 2600
SFRField<uint32_t> acceptable_battery =
    SFRField<uint32_t>(3.75, 3, 4, 0x2600, 100);
SFRField<uint32_t> min_battery = SFRField<uint32_t>(0, 0x2601);

SensorReading *voltage_value =
    new SensorReading(fault_groups::power_faults::voltage_value, 1, 0, 5.5);
SensorReading *voltage_average =
    new SensorReading(fault_groups::power_faults::voltage_average, 300, 0, 5.5);
}  // namespace battery
namespace button {
// OP Codes 2700
SFRField<bool> pressed = SFRField<bool>(true, 0x2700);

SensorReading *button_pressed =
    new SensorReading(fault_groups::hardware_faults::button, 1, 0, 1);
}  // namespace button
namespace pins {
std::map<int, int> pinMap = {{constants::acs::xPWMpin, LOW},
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
}  // namespace pins
namespace eeprom {
// OP Codes 2800
SFRField<bool> allotted_time_passed = SFRField<bool>(
    false,
    0x2800);  // indicates if the EEPROM count has reached the alloted time
SFRField<bool> storage_full = SFRField<bool>(false, 0x2801);
SFRField<uint8_t> boot_counter = SFRField<uint8_t>(0, 0x2802);
SFRField<uint32_t> sfr_write_step_time = SFRField<uint32_t>(
    1000,
    0x2805);  // the amount of time between each write to EEPROM for SFR data
SFRField<uint32_t> sfr_address_age = SFRField<uint32_t>(
    0, 0x2806);  // the write age of the current SFR data section in EEPROM

int sfr_last_write_cycle =
    0;  // the millis() value when the last EEPROM SFR data write ocurred,
        // should reset every boot up cycle
uint16_t sfr_address =
    7;  // the address of where current SFR data is stored, read from EEPROM and
        // set at the beginning of every boot up cycle
// Bytes 0-3 are for the time passed, byte 4 holds the number of reboots, and
// bytes 5-6 are for the address where values are to be written and stored. SFR
// data begins at byte 7 and after. The section of EEPROM bytes where SFR data
// is stored will change to avoid exceeding the write endurance.
}  // namespace eeprom
};  // namespace sfr

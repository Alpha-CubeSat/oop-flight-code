#include "MissionMode.hpp"
#include "sfr.hpp"

void boot_initialization()
{
    settings(
        true,                        // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::lp_downlink_period);
    sfr::imu::failed_times = 0;
}

void Boot::transition_to()
{
    boot_initialization();
}
void Boot::dispatch()
{
    if (sfr::eeprom::time_alive >= sfr::boot::max_time) {
        sfr::mission::current_mode = sfr::mission::aliveSignal;
    }
}

void AliveSignal::transition_to()
{
    settings(
        false,                       // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::transmit_downlink_period);
}
void AliveSignal::dispatch()
{
    // if rockblock hard faults 3 times exit alive signal
    if (sfr::aliveSignal::num_hard_faults >= sfr::aliveSignal::max_downlink_hard_faults || sfr::aliveSignal::downlinked) {
        sfr::aliveSignal::downlinked = false;
        sfr::aliveSignal::num_hard_faults = 0;
        exit_alive_signal();
    }
    if (millis() - sfr::mission::aliveSignal->start_time >= sfr::aliveSignal::max_time) {
        exit_alive_signal();
    }
}

void DetumbleSpin::transition_to()
{
    settings(
        true,                        // rockblock sleeping
        sensor_power_mode_type::off, // camera
        false,                       // acs off
        sfr::rockblock::transmit_downlink_period);
    sfr::acs::mode = (uint8_t)acs_mode_type::detumble;
}
void DetumbleSpin::dispatch()
{
    exit_detumble_phase(sfr::mission::normal);
    enter_lp(sfr::mission::lowPowerDetumbleSpin);
}

void LowPowerDetumbleSpin::transition_to()
{
    settings(
        true,                        // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::lp_downlink_period);
}
void LowPowerDetumbleSpin::dispatch()
{
    exit_lp(sfr::mission::detumbleSpin);
    exit_detumble_phase(sfr::mission::lowPower);
}

void Normal::transition_to()
{
    settings(
        true,                        // rockblock sleeping
        sensor_power_mode_type::off, // camera
        false,                       // acs off
        sfr::rockblock::transmit_downlink_period);
}
void Normal::dispatch()
{
    timed_out(sfr::mission::transmit, sfr::acs::on_time);
    enter_lp(sfr::mission::lowPower); // entering lp takes precedence
}

void LowPower::transition_to()
{
    settings(
        false,                       // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::lp_downlink_period);
}
void LowPower::dispatch()
{
    check_previous(sfr::mission::normal, sfr::mission::transmit);
}

void Transmit::transition_to()
{
    settings(
        false,                       // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::transmit_downlink_period);
}
void Transmit::dispatch()
{
    timed_out(sfr::mission::normal, sfr::rockblock::on_time);
    enter_lp(sfr::mission::lowPower); // entering lp takes precedence
}

void NormalDeployment::transition_to()
{
    settings(
        true,                        // rockblock sleeping
        sensor_power_mode_type::off, // camera
        false,                       // acs off
        sfr::rockblock::transmit_downlink_period);
}
void NormalDeployment::dispatch()
{
    timed_out(sfr::mission::transmitDeployment, sfr::acs::on_time);
    enter_lp(sfr::mission::lowPowerDeployment); // entering lp takes precedence
}

void TransmitDeployment::transition_to()
{
    settings(
        false,                       // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::transmit_downlink_period);
}
void TransmitDeployment::dispatch()
{
    timed_out(sfr::mission::normalDeployment, sfr::rockblock::on_time);
    enter_lp(sfr::mission::lowPowerDeployment); // entering lp takes precedence
}

void LowPowerDeployment::transition_to()
{
    settings(
        false,                       // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::lp_downlink_period);
}
void LowPowerDeployment::dispatch()
{
    check_previous(sfr::mission::normalDeployment, sfr::mission::transmitDeployment);
}

void NormalArmed::transition_to()
{
    settings(
        true,                        // rockblock sleeping
        sensor_power_mode_type::off, // camera
        false,                       // acs off
        sfr::rockblock::transmit_downlink_period);
}
void NormalArmed::dispatch()
{
    timed_out(sfr::mission::transmitArmed, sfr::acs::on_time);
    enter_lp(sfr::mission::lowPowerArmed); // entering lp takes precedence
}

void TransmitArmed::transition_to()
{
    settings(
        false,                       // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::transmit_downlink_period);
}
void TransmitArmed::dispatch()
{
    timed_out(sfr::mission::normalArmed, sfr::rockblock::on_time);
    enter_lp(sfr::mission::lowPowerArmed); // entering lp takes precedence
}

void LowPowerArmed::transition_to()
{
    settings(
        false,                       // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::lp_downlink_period);
}
void LowPowerArmed::dispatch()
{
    check_previous(sfr::mission::normalArmed, sfr::mission::transmitArmed);
}

void NormalInSun::transition_to()
{
    settings(
        true,                        // rockblock sleeping
        sensor_power_mode_type::off, // camera
        false,                       // acs off
        sfr::rockblock::transmit_downlink_period);
}
void NormalInSun::dispatch()
{
    timed_out(sfr::mission::transmitInSun, sfr::acs::on_time);
    exit_insun_phase(sfr::mission::bootIMU);
    enter_lp_insun(); // entering lp takes precedence
}

void TransmitInSun::transition_to()
{
    settings(
        false,                       // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::transmit_downlink_period);
}
void TransmitInSun::dispatch()
{
    timed_out(sfr::mission::normalInSun, sfr::rockblock::on_time);
    exit_insun_phase(sfr::mission::bootIMU);
    enter_lp_insun(); // entering lp takes precedence
}

void LowPowerInSun::transition_to()
{
    settings(
        false,                       // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::lp_downlink_period);
}
void LowPowerInSun::dispatch()
{
    if (!sfr::battery::voltage_average->is_valid()) {
        sfr::mission::current_mode = sfr::mission::voltageFailureInSun;
    } else {
        check_previous(sfr::mission::normalInSun, sfr::mission::transmitInSun);
    }
}

void VoltageFailureInSun::transition_to()
{
    settings(
        false,                       // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::lp_downlink_period);
}

void VoltageFailureInSun::dispatch()
{
    if (sfr::battery::voltage_average->is_valid()) {
        sfr::mission::current_mode = sfr::mission::normalInSun;
    } else {
        exit_insun_phase(sfr::mission::bootIMU);
    }
}

void BootIMU::transition_to()
{
    settings(
        true,                        // rockblock sleeping
        sensor_power_mode_type::off, // camera
        true,                        // acs off
        sfr::rockblock::lp_downlink_period);
    sfr::imu::sample_gyro = true;
    sfr::imu::failed_times = 0;
}
void BootIMU::dispatch()
{
    // Once the IMU is initialized and 20 seconds have passed transition.
    // OR If the failed tiems exceeds the limit transition
    if (((sfr::imu::init_mode == (uint16_t)sensor_init_mode_type::complete) && ((millis() - sfr::imu::imu_boot_collection_start_time) >= constants::imu::boot_IMU_min_run_time)) || sfr::imu::failed_times >= sfr::imu::failed_limit) {
        sfr::mission::current_mode = sfr::mission::bootCamera;
    }
}

void BootCamera::transition_to()
{
    settings(
        true,                       // rockblock sleeping
        sensor_power_mode_type::on, // camera
        true,                       // acs off
        sfr::rockblock::lp_downlink_period);
}

void BootCamera::dispatch()
{
    if (sfr::camera::init_mode == (uint16_t)sensor_init_mode_type::complete || sfr::camera::failed_times >= sfr::camera::failed_limit) {
        sfr::mission::current_mode = sfr::mission::mandatoryBurns;
    }
}

void MandatoryBurns::transition_to()
{
    settings(
        true,                               // rockblock sleeping
        sensor_power_mode_type::do_nothing, // camera
        true,                               // acs off
        sfr::rockblock::lp_downlink_period);
    sfr::mission::possible_uncovered = true;
}

void MandatoryBurns::dispatch()
{
    if (sfr::burnwire::attempts > sfr::burnwire::mandatory_attempts_limit) {
        sfr::mission::current_mode = sfr::mission::regularBurns;
    }
}

void RegularBurns::transition_to()
{
    settings(
        true,                               // rockblock sleeping
        sensor_power_mode_type::do_nothing, // camera
        true,                               // acs off
        sfr::rockblock::lp_downlink_period);
}

void RegularBurns::dispatch()
{
    if ((!sfr::button::pressed && !fault_groups::hardware_faults::button->get_base()) || (!sfr::photoresistor::covered && !fault_groups::hardware_faults::light_val->get_base())) {
        sfr::mission::current_mode = sfr::mission::photo;
        sfr::mission::deployed = true;

    } else if (sfr::burnwire::attempts > sfr::burnwire::attempts_limit) {
        sfr::mission::current_mode = sfr::mission::transmitArmed;
    }
}

void Photo::transition_to()
{
    settings(
        true,                               // rockblock sleeping
        sensor_power_mode_type::do_nothing, // camera
        true,                               // acs off
        sfr::rockblock::lp_downlink_period);
    sfr::camera::take_photo = true;
}

void Photo::dispatch()
{
    // Only go onto the next state until the IMU finished collecting all of the data
    if (millis() - sfr::imu::door_open_start_time > constants::imu::door_open_end_time) {
        sfr::mission::current_mode = sfr::mission::detumbleSpin;
    }
}

void exit_signal_phase(MissionMode *mode)
{
    // if rockblock hard faults 3 times exit alive signal
    if (sfr::aliveSignal::num_hard_faults >= sfr::aliveSignal::max_downlink_hard_faults || sfr::aliveSignal::downlinked) {
        sfr::mission::current_mode = mode;
        sfr::aliveSignal::downlinked = false;
        sfr::aliveSignal::num_hard_faults = 0;
    }
    if (millis() - sfr::mission::aliveSignal->start_time >= sfr::aliveSignal::max_time) {
        sfr::mission::current_mode = mode;
    }
}

void exit_detumble_phase(MissionMode *mode)
{

    // TODO min stable/unstable gyro and max stable gyro are SFR fields with resolution. FS-160
    float gyro_x;
    float gyro_y;
    float gyro_z;

    // invalid readings from IMU
    if (sfr::imu::failed_times >= sfr::imu::failed_limit) {
        sfr::mission::current_mode = mode;
        sfr::acs::mode = (uint8_t)acs_mode_type::point;
    }

    // cubesat has stabilized: gyro z > 0.9 rad/s && gyro x and gyro y are below 0.1 rad/s
    if (sfr::imu::gyro_z_average->get_value(&gyro_z) && gyro_z >= sfr::detumble::min_stable_gyro_z.get_float() &&
        sfr::imu::gyro_x_average->get_value(&gyro_x) && gyro_x <= sfr::detumble::max_stable_gyro_x.get_float() &&
        sfr::imu::gyro_y_average->get_value(&gyro_y) && gyro_y <= sfr::detumble::max_stable_gyro_y.get_float()) {
        sfr::mission::current_mode = mode;
        sfr::acs::mode = (uint8_t)acs_mode_type::point;
    }

    // cubesat will never stabilize: x gyro or y gyro are greater than 0.7 rad/s
    if ((sfr::imu::gyro_x_average->get_value(&gyro_x) && gyro_x >= sfr::detumble::min_unstable_gyro_x.get_float()) ||
        (sfr::imu::gyro_y_average->get_value(&gyro_y) && gyro_y >= sfr::detumble::min_unstable_gyro_y.get_float())) {
        sfr::mission::current_mode = mode;
        sfr::acs::mode = (uint8_t)acs_mode_type::point;
    }

    // detumble has timed out
    if (millis() - sfr::mission::stabilization->start_time >= sfr::stabilization::max_time) {
        sfr::mission::current_mode = mode;
        sfr::acs::mode = (uint8_t)acs_mode_type::point;
    }
}

void exit_insun_phase(MissionMode *mode)
{
    if ((sfr::temperature::temp_c_average->is_valid() && sfr::temperature::in_sun) ||
        (!sfr::temperature::temp_c_average->is_valid() && sfr::current::solar_current_average->is_valid() && sfr::current::in_sun) ||
        (!sfr::temperature::temp_c_average->is_valid() && !sfr::current::solar_current_average->is_valid())) {
        sfr::mission::current_mode = mode;
    }
}

void enter_lp(MissionMode *lp_mode)
{
    float voltage;
    if (!sfr::battery::voltage_average->is_valid() || (sfr::battery::voltage_average->get_value(&voltage) && voltage <= sfr::battery::min_battery.get_float())) {
        sfr::mission::current_mode = lp_mode;
    }
}

void check_previous(MissionMode *normal_mode, MissionMode *transmit_mode)
{
    if (sfr::mission::mode_history[1] == transmit_mode->get_id()) {
        exit_lp(transmit_mode);
    } else {
        exit_lp(normal_mode);
    }
}

void exit_lp(MissionMode *reg_mode)
{
    float voltage;

    if (sfr::battery::voltage_average->get_value(&voltage) && voltage > sfr::battery::acceptable_battery.get_float()) {
        sfr::mission::current_mode = reg_mode;
    }
}

void timed_out(MissionMode *next_mode, uint32_t max_time)
{
    if (millis() - sfr::mission::current_mode->start_time >= max_time) {
        sfr::mission::current_mode = next_mode;
    }
}

void enter_lp_insun()
{
    float voltage;

    // using is valid rather than the return of get value because we should not enter voltageFailureInSun just because the buffer is not full

    if (!sfr::battery::voltage_average->is_valid()) {
        sfr::mission::current_mode = sfr::mission::voltageFailureInSun;
    } else if (sfr::battery::voltage_average->get_value(&voltage) && voltage <= sfr::battery::min_battery.get_float()) {
        sfr::mission::current_mode = sfr::mission::lowPowerInSun;
    }
}

void settings(bool rockblock_sleep_mode, sensor_power_mode_type camera_power_setting, bool acs_off, uint32_t downlink_period)
{
    sfr::rockblock::sleep_mode = rockblock_sleep_mode;
    sfr::camera::power_setting = (uint8_t)camera_power_setting;
    sfr::acs::off = acs_off;
    sfr::rockblock::downlink_period = downlink_period;
    sfr::imu::power_setting = (uint8_t)sensor_power_mode_type::on;
}

void exit_alive_signal()
{
    if (sfr::eeprom::time_alive >= (sfr::boot::max_time + sfr::stabilization::max_time)) {
        sfr::mission::current_mode = sfr::mission::normal;
        if (sfr::acs::mode == (uint8_t)acs_mode_type::detumble) {
            sfr::acs::mode = (uint8_t)acs_mode_type::point;
        }
    } else {
        sfr::mission::current_mode = sfr::mission::detumbleSpin;
    }
}

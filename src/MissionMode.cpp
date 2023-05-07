#include "MissionMode.hpp"
#include "sfr.hpp"

void boot_initialization()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::turn_off = true;
}

void Boot::transition_to()
{
    boot_initialization();
}
void Boot::dispatch()
{
    timed_out(sfr::mission::aliveSignal, sfr::boot::max_time);
}

void AliveSignal::transition_to()
{
    transmit_mode_settings();
}
void AliveSignal::dispatch()
{
    enter_lp(sfr::mission::lowPowerAliveSignal);
    exit_signal_phase(sfr::mission::detumbleSpin);
}

void LowPowerAliveSignal::transition_to()
{
    transmit_mode_settings();
}
void LowPowerAliveSignal::dispatch()
{
    exit_lp(sfr::mission::aliveSignal);
    exit_signal_phase(sfr::mission::lowPowerDetumbleSpin);
}

void DetumbleSpin::transition_to()
{
    acs_mode_settings();
}
void DetumbleSpin::dispatch()
{
    if (sfr::imu::failed_times > sfr::imu::failed_limit) {
        sfr::mission::current_mode = sfr::mission::normal;
    }
    exit_detumble_phase(sfr::mission::normal);
    timed_out(sfr::mission::normal, sfr::acs::detumble_timeout);
}

void LowPowerDetumbleSpin::transition_to()
{
    transmit_mode_settings();
}
void LowPowerDetumbleSpin::dispatch()
{
    exit_lp(sfr::mission::detumbleSpin);
    exit_detumble_phase(sfr::mission::lowPower);
}

void Normal::transition_to()
{
    acs_mode_settings();
}
void Normal::dispatch()
{

    enter_lp(sfr::mission::lowPower);
    timed_out(sfr::mission::transmit, sfr::acs::on_time);
}

void LowPower::transition_to()
{
    transmit_mode_settings();
}
void LowPower::dispatch()
{
    check_previous(sfr::mission::normal, sfr::mission::transmit);
}

void Transmit::transition_to()
{
    transmit_mode_settings();
}
void Transmit::dispatch()
{
    enter_lp(sfr::mission::lowPower);
    timed_out(sfr::mission::normal, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
}

void NormalDeployment::transition_to()
{
    acs_mode_settings();
}
void NormalDeployment::dispatch()
{
    enter_lp(sfr::mission::lowPowerDeployment);
    timed_out(sfr::mission::transmitDeployment, sfr::acs::on_time);
}

void TransmitDeployment::transition_to()
{
    transmit_mode_settings();
}
void TransmitDeployment::dispatch()
{
    enter_lp(sfr::mission::lowPowerDeployment);
    timed_out(sfr::mission::normalDeployment, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
}

void LowPowerDeployment::transition_to()
{
    transmit_mode_settings();
}
void LowPowerDeployment::dispatch()
{
    check_previous(sfr::mission::normalDeployment, sfr::mission::transmitDeployment);
}

void NormalArmed::transition_to()
{
    acs_mode_settings();
}
void NormalArmed::dispatch()
{
    enter_lp(sfr::mission::lowPowerArmed);
    timed_out(sfr::mission::transmitArmed, sfr::acs::on_time);
}

void TransmitArmed::transition_to()
{
    transmit_mode_settings();
    sfr::burnwire::attempts = 0;
}
void TransmitArmed::dispatch()
{
    enter_lp(sfr::mission::lowPowerArmed);
    timed_out(sfr::mission::normalArmed, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
}

void LowPowerArmed::transition_to()
{
    transmit_mode_settings();
}
void LowPowerArmed::dispatch()
{
    check_previous(sfr::mission::normalArmed, sfr::mission::transmitArmed);
}

void NormalInSun::transition_to()
{
    acs_mode_settings();
}
void NormalInSun::dispatch()
{
    timed_out(sfr::mission::transmitInSun, sfr::acs::on_time);
    enter_lp_insun();
    exit_insun_phase(sfr::mission::bootImu);
}

void TransmitInSun::transition_to()
{
    transmit_mode_settings();
}
void TransmitInSun::dispatch()
{
    timed_out(sfr::mission::normalInSun, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
    enter_lp_insun();
    exit_insun_phase(sfr::mission::bootImu);
}

void LowPowerInSun::transition_to()
{
    transmit_mode_settings();
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
    transmit_mode_settings();
}

void VoltageFailureInSun::dispatch()
{
    if (sfr::battery::voltage_average->is_valid()) {
        sfr::mission::current_mode = sfr::mission::normalInSun;
    } else {
        exit_insun_phase(sfr::mission::bootImu);
    }
}

void BootIMU::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::turn_on = true;
    sfr::imu::turn_off = false;
}
void BootIMU::dispatch()
{
    // sfr::mission::current_mode = sfr::mission::bootImu;
    // this is where we need to do the 20 seconds
    if (((sfr::imu::init_mode == (uint16_t)sensor_init_mode_type::complete) && ((millis() - sfr::imu::imu_boot_collection_start_time) >= constants::imu::bootIMU_min_run_time)) || sfr::imu::failed_times >= sfr::camera::failed_limit) {
        sfr::mission::current_mode = sfr::mission::bootCamera;
        // reset failed times once we transition
        sfr::imu::failed_times = 0;
    }
}

void BootCamera::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::turn_on = true;
    sfr::camera::turn_on = true;
}

void BootCamera::dispatch()
{
    if (sfr::camera::init_mode == (uint16_t)sensor_init_mode_type::complete || sfr::camera::failed_times > sfr::camera::failed_limit) {
        sfr::mission::current_mode = sfr::mission::mandatoryBurns;
    }
}

void MandatoryBurns::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::mission::possible_uncovered = true;
#ifdef E2E_TESTING
    sfr::rockblock::downlink_period = 30 * constants::time::one_minute;
#endif
}

void MandatoryBurns::dispatch()
{
    if (sfr::burnwire::attempts > sfr::burnwire::mandatory_attempts_limit) {
        sfr::mission::current_mode = sfr::mission::regularBurns;
    }
}

void RegularBurns::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
}

void RegularBurns::dispatch()
{
    if (!sfr::button::pressed || !sfr::photoresistor::covered) {
        sfr::mission::current_mode = sfr::mission::photo;

    } else if (sfr::burnwire::attempts > sfr::burnwire::attempts_limit) {
        sfr::mission::current_mode = sfr::mission::transmitArmed;
    }
}

void Photo::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::camera::take_photo = true;
}

void Photo::dispatch()
{
    // Only go onto the next state until the IMU finished collecting all of the data
    if (millis() > sfr::imu::door_open__collection_start_time + constants::imu::after_door_opens_min_run_time) {
        sfr::mission::current_mode = sfr::mission::detumbleSpin;
        sfr::imu::turn_off = true;
    }
}

void exit_signal_phase(MissionMode *mode)
{
    // if rockblock hard faults 3 times exit alive signal
    if (sfr::aliveSignal::num_hard_faults >= sfr::aliveSignal::max_downlink_hard_faults || sfr::aliveSignal::downlinked) {
        sfr::mission::current_mode = mode;
    }
    // if rockblock cannot get signal in one minute exit alive signal
    if (millis() - sfr::rockblock::start_time_check_signal >= sfr::rockblock::max_check_signal_time) {
        sfr::mission::current_mode = mode;
    }
    // if signal phase has timed out
    timed_out(mode, sfr::aliveSignal::max_time);
}

void exit_detumble_phase(MissionMode *mode)
{
    // TODO min stable/unstable gyro and max stable gyro are SFR fields with resolution. FS-160
    float gyro_x;
    float gyro_y;
    float gyro_z;

    // cubesat has stabilized: gyro z > 1 rad/s && gyro x and gyro y are below 0.2 rad/s
    if (sfr::imu::gyro_z_average->is_valid() && sfr::imu::gyro_x_average->is_valid() && sfr::imu::gyro_y_average->is_valid() &&
        sfr::imu::gyro_z_average->get_value(&gyro_z) && gyro_z >= sfr::detumble::min_stable_gyro_z.get_float() &&
        sfr::imu::gyro_x_average->get_value(&gyro_x) && gyro_x <= sfr::detumble::max_stable_gyro_x.get_float() &&
        sfr::imu::gyro_y_average->get_value(&gyro_y) && gyro_y <= sfr::detumble::max_stable_gyro_y.get_float()) {
        sfr::mission::current_mode = mode;
    }

    // cubesat will never stabilize: x gyro or y gyro are greater than 0.7 rad/s
    if (sfr::imu::gyro_x_average->is_valid() && sfr::imu::gyro_y_average->is_valid() &&
        ((sfr::imu::gyro_x_average->get_value(&gyro_x) && gyro_x >= sfr::detumble::min_unstable_gyro_x.get_float()) ||
         (sfr::imu::gyro_y_average->get_value(&gyro_y) && gyro_y >= sfr::detumble::min_unstable_gyro_y.get_float()))) {
        sfr::mission::current_mode = mode;
    }

    // detumble has timed out
    if (millis() - sfr::mission::stabilization->start_time >= sfr::stabilization::max_time) {
        sfr::mission::current_mode = mode;
    }
}

void exit_insun_phase(MissionMode *mode)
{
    if ((sfr::temperature::temp_c_average->is_valid() && sfr::temperature::in_sun) ||
        (!sfr::temperature::temp_c_average->is_valid() && sfr::current::solar_current_average->is_valid() && sfr::current::in_sun)) {
        sfr::mission::current_mode = mode;
    }
}

void enter_lp(MissionMode *lp_mode)
{
    float voltage;

#ifndef E2E_TESTING
    if (!sfr::battery::voltage_average->is_valid() || (sfr::battery::voltage_average->get_value(&voltage) && voltage <= sfr::battery::min_battery)) {
        sfr::mission::current_mode = lp_mode;
    }
#endif

#ifdef E2E_TESTING
    if (!sfr::battery::voltage_average->is_valid() || (sfr::battery::voltage_value->get_value(&voltage) && voltage <= sfr::battery::min_battery)) {
        sfr::mission::current_mode = lp_mode;
    }
#endif
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

    Serial.println(sfr::battery::voltage_average->get_value(&voltage));

    if (sfr::battery::voltage_average->get_value(&voltage) && voltage > sfr::battery::acceptable_battery) {
        Serial.println(voltage);
        sfr::mission::current_mode = reg_mode;
    }
}

void timed_out(MissionMode *next_mode, float max_time)
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
    } else if (sfr::battery::voltage_average->get_value(&voltage) && voltage <= sfr::battery::min_battery) {
        sfr::mission::current_mode = sfr::mission::lowPowerInSun;
    }
}

void transmit_mode_settings()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::turn_off = true;
}

void acs_mode_settings()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = false;
    sfr::imu::turn_on = true;
}

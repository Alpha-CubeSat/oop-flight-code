#include "MissionMode.hpp"
#include "sfr.hpp"

void boot_initialization()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}

void Boot::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void Boot::dispatch()
{
    timed_out(sfr::mission::aliveSignal, sfr::boot::max_time);
}

void AliveSignal::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void AliveSignal::dispatch()
{
    enter_lp(sfr::mission::lowPowerAliveSignal);
    exit_signal_phase(sfr::mission::detumbleSpin);
}

void LowPowerAliveSignal::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void LowPowerAliveSignal::dispatch()
{
    exit_lp(sfr::mission::aliveSignal);
    exit_signal_phase(sfr::mission::lowPowerDetumbleSpin);
}

void DetumbleSpin::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = false;
    sfr::imu::sample = true;
}
void DetumbleSpin::dispatch()
{
    if (sfr::imu::mode == (uint16_t)sensor_mode_type::abnormal_init) {
        sfr::imu::mode = (uint16_t)sensor_mode_type::retry;
        sfr::detumble::num_imu_retries++;
    }
    if (sfr::detumble::num_imu_retries >= sfr::detumble::max_imu_retries) {
        sfr::mission::current_mode = sfr::mission::normal;
    }
    enter_lp(sfr::mission::lowPowerDetumbleSpin);
    exit_detumble_phase(sfr::mission::normal);
}

void LowPowerDetumbleSpin::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void LowPowerDetumbleSpin::dispatch()
{
    exit_lp(sfr::mission::detumbleSpin);
    exit_detumble_phase(sfr::mission::lowPower);
}

void Normal::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = false;
    sfr::imu::sample = true;
}
void Normal::dispatch()
{
    enter_lp(sfr::mission::lowPower);
    timed_out(sfr::mission::transmit, sfr::acs::on_time);
}

void LowPower::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void LowPower::dispatch()
{
    check_previous(sfr::mission::normal, sfr::mission::transmit);
}

void Transmit::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void Transmit::dispatch()
{
    enter_lp(sfr::mission::lowPower);
    timed_out(sfr::mission::normal, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
}

void NormalDeployment::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = false;
    sfr::imu::sample = true;
}
void NormalDeployment::dispatch()
{
    enter_lp(sfr::mission::lowPowerDeployment);
    timed_out(sfr::mission::transmitDeployment, sfr::acs::on_time);
}

void TransmitDeployment::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void TransmitDeployment::dispatch()
{
    enter_lp(sfr::mission::lowPowerDeployment);
    timed_out(sfr::mission::normalDeployment, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
}

void LowPowerDeployment::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void LowPowerDeployment::dispatch()
{
    check_previous(sfr::mission::normalDeployment, sfr::mission::transmitDeployment);
}

void NormalArmed::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = false;
    sfr::imu::sample = true;
}
void NormalArmed::dispatch()
{
    enter_lp(sfr::mission::lowPowerArmed);
    timed_out(sfr::mission::transmitArmed, sfr::acs::on_time);
}

void TransmitArmed::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void TransmitArmed::dispatch()
{
    enter_lp(sfr::mission::lowPowerArmed);
    timed_out(sfr::mission::normalArmed, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
}

void LowPowerArmed::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void LowPowerArmed::dispatch()
{
    check_previous(sfr::mission::normalArmed, sfr::mission::transmitArmed);
}

void NormalInSun::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = false;
    sfr::imu::sample = true;
}
void NormalInSun::dispatch()
{
    timed_out(sfr::mission::transmitInSun, sfr::acs::on_time);
    enter_lp_insun();
}

void TransmitInSun::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void TransmitInSun::dispatch()
{
    timed_out(sfr::mission::normalInSun, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
    enter_lp_insun();
}

void LowPowerInSun::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::sample = false;
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
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void VoltageFailureInSun::dispatch()
{
    if (sfr::battery::voltage_average->is_valid()) {
        sfr::mission::current_mode = sfr::mission::normalInSun;
    }
}

void BootCamera::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::sample = false;
}
void BootCamera::dispatch()
{
}

void MandatoryBurns::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::sample = true;
}
void MandatoryBurns::dispatch()
{
}

void RegularBurns::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::sample = true;
}
void RegularBurns::dispatch()
{
}

void Photo::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::sample = true;
}
void Photo::dispatch() {}

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
    // TODO min unstable gyro and max unstable gyro are SFR fields with resolution. FS-160
    /*
    // cubesat has stabilized: gyro z > 1 rad/s && gyro x and gyro y are below 0.2 rad/s
    if (sfr::imu::gyro_z_average->is_valid() && sfr::imu::gyro_x_average->is_valid() && sfr::imu::gyro_y_average->is_valid() && (sfr::imu::gyro_z_average->get_value() >= sfr::detumble::min_stable_gyro_z) && (sfr::imu::gyro_x_average->get_value() <= sfr::detumble::max_stable_gyro_x) && (sfr::imu::gyro_y_average->get_value() <= sfr::detumble::max_stable_gyro_y)) {
        sfr::mission::current_mode = mode;
    }

    // cubesat will never stabilize: x gyro or y gyro are greater than 0.7 rad/s
    if (sfr::imu::gyro_x_average->is_valid() && sfr::imu::gyro_y_average->is_valid() &&
        (sfr::imu::gyro_x_average->get_value() >= sfr::detumble::min_unstable_gyro_x) &&
        (sfr::imu::gyro_y_average->get_value() >= sfr::detumble::min_unstable_gyro_y)) {
        sfr::mission::current_mode = mode;
    }*/

    // detumble has timed out
    if (millis() - sfr::mission::stabilization->start_time >= sfr::stabilization::max_time) {
        sfr::mission::current_mode = mode;
    }
}

void enter_lp(MissionMode *lp_mode)
{
    float voltage;

    if (!sfr::battery::voltage_average->is_valid() || (sfr::battery::voltage_average->get_value(&voltage) && voltage <= sfr::battery::min_battery)) {
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

    if (sfr::battery::voltage_average->get_value(&voltage) && voltage >= sfr::battery::acceptable_battery) {
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

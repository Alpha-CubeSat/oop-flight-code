#include "MissionMode.hpp"
#include "sfr.hpp"

void boot_initialization() {}

void Boot::transition_to() {}
void Boot::dispatch()
{
    timed_out(sfr::mission::aliveSignal, sfr::boot::max_time);
}

void AliveSignal::transition_to() {}
void AliveSignal::dispatch()
{
    enter_lp(sfr::mission::lowPowerAliveSignal);
    exit_signal_phase(sfr::mission::detumbleSpin);
}

void LowPowerAliveSignal::transition_to() {}
void LowPowerAliveSignal::dispatch()
{
    exit_lp(sfr::mission::aliveSignal);
    exit_signal_phase(sfr::mission::lowPowerDetumbleSpin);
}

void DetumbleSpin::transition_to() {}
void DetumbleSpin::dispatch()
{
    if (sfr::imu::mode == sensor_mode_type::abnormal_init) {
        sfr::imu::mode = retry;
        sfr::detumble::num_imu_retries++;
    }
    if (sfr::detumble::num_imu_retries >= sfr::detumble::max_imu_retries) {
        sfr::mission::current_mode = sfr::mission::normal;
    }
    enter_lp(sfr::mission::lowPowerDetumbleSpin);
    exit_detumble_phase(sfr::mission::normal);
}

void LowPowerDetumbleSpin::transition_to() {}
void LowPowerDetumbleSpin::dispatch()
{
    exit_lp(sfr::mission::detumbleSpin);
    exit_detumble_phase(sfr::mission::lowPower);
}

void Normal::transition_to()
{
    sfr::mission::normal->set_start_time(millis());
}
void Normal::dispatch()
{
    enter_lp(sfr::mission::lowPower);
    timed_out(sfr::mission::transmit, sfr::acs::on_time);
}

void LowPower::transition_to() {}
void LowPower::dispatch()
{
    check_previous(sfr::mission::normal, sfr::mission::transmit);
}

void Transmit::transition_to()
{
    sfr::mission::transmit->set_start_time(millis());
}
void Transmit::dispatch()
{
    enter_lp(sfr::mission::lowPower);
    timed_out(sfr::mission::normal, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
}

void NormalDeployment::transition_to()
{
    sfr::mission::normalDeployment->set_start_time(millis());
}
void NormalDeployment::dispatch()
{
    enter_lp(sfr::mission::lowPowerDeployment);
    timed_out(sfr::mission::transmitDeployment, sfr::acs::on_time);
}

void TransmitDeployment::transition_to()
{
    sfr::mission::transmitDeployment->set_start_time(millis());
}
void TransmitDeployment::dispatch()
{
    enter_lp(sfr::mission::lowPowerDeployment);
    timed_out(sfr::mission::normalDeployment, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
}

void LowPowerDeployment::transition_to() {}
void LowPowerDeployment::dispatch()
{
    check_previous(sfr::mission::normalDeployment, sfr::mission::transmitDeployment);
}

void NormalArmed::transition_to()
{
    sfr::mission::normalArmed->set_start_time(millis());
}
void NormalArmed::dispatch()
{
    enter_lp(sfr::mission::lowPowerArmed);
    timed_out(sfr::mission::transmitArmed, sfr::acs::on_time);
}

void TransmitArmed::transition_to()
{
    sfr::mission::transmitArmed->set_start_time(millis());
}
void TransmitArmed::dispatch()
{
    enter_lp(sfr::mission::lowPowerArmed);
    timed_out(sfr::mission::normalArmed, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
}

void LowPowerArmed::transition_to() {}
void LowPowerArmed::dispatch()
{
    check_previous(sfr::mission::normalArmed, sfr::mission::transmitArmed);
}

void NormalInSun::transition_to() {}
void NormalInSun::dispatch()
{
}

void TransmitInSun::transition_to() {}
void TransmitInSun::dispatch()
{
}

void LowPowerInSun::transition_to() {}
void LowPowerInSun::dispatch()
{
}

void VoltageFailureInSun::transition_to() {}
void VoltageFailureInSun::dispatch()
{
}

void BootCamera::transition_to() {}
void BootCamera::dispatch()
{
}

void MandatoryBurns::transition_to() {}
void MandatoryBurns::dispatch()
{
}

void RegularBurns::transition_to() {}
void RegularBurns::dispatch()
{
}

void Photo::transition_to() {}
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
    // cubesat has stabilized: gyro z > 1 rad/s && gyro x and gyro y are below 0.2 rad/s
    if (sfr::imu::gyro_z_average->is_valid() && sfr::imu::gyro_x_average->is_valid() && sfr::imu::gyro_y_average->is_valid() && (sfr::imu::gyro_z_average->get_value() >= sfr::detumble::min_stable_gyro_z) && (sfr::imu::gyro_x_average->get_value() <= sfr::detumble::max_stable_gyro_x) && (sfr::imu::gyro_y_average->get_value() <= sfr::detumble::max_stable_gyro_y)) {
        sfr::mission::current_mode = mode;
    }

    // cubesat will never stabilize: x gyro or y gyro are greater than 0.7 rad/s
    if (sfr::imu::gyro_x_average->is_valid() && sfr::imu::gyro_y_average->is_valid() &&
        (sfr::imu::gyro_x_average->get_value() >= sfr::detumble::min_unstable_gyro_x) &&
        (sfr::imu::gyro_y_average->get_value() >= sfr::detumble::min_unstable_gyro_y)) {
        sfr::mission::current_mode = mode;
    }

    // detumble has timed out
    if (millis() - sfr::detumble::start_time >= sfr::detumble::max_time) {
        sfr::mission::current_mode = mode;
    }
}

void enter_lp(MissionMode *lp_mode)
{
    if (!sfr::battery::voltage_average->is_valid() || sfr::battery::voltage_average->get_value() <= sfr::battery::min_battery) {
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
    if (sfr::battery::voltage_average->is_valid() && sfr::battery::voltage_average->get_value() >= sfr::battery::acceptable_battery) {
        sfr::mission::current_mode = reg_mode;
    }
}

void timed_out(MissionMode *next_mode, float max_time)
{
    if (millis() - sfr::mission::current_mode->start_time >= max_time) {
        sfr::mission::current_mode = next_mode;
    }
}

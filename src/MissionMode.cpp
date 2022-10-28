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
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::turn_off = true;
}
void Boot::dispatch()
{
    timed_out(sfr::mission::aliveSignal, sfr::boot::max_time);
}

void AliveSignal::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::turn_off = true;
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
    sfr::imu::turn_off = true;
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
    sfr::imu::turn_off = true;
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
    sfr::imu::turn_off = true;
}
void LowPower::dispatch()
{
    check_previous(sfr::mission::normal, sfr::mission::transmit);
}

void Transmit::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::turn_off = true;
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
    sfr::imu::turn_off = true;
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
    sfr::imu::turn_off = true;
}
void LowPowerDeployment::dispatch()
{
    check_previous(sfr::mission::normalDeployment, sfr::mission::transmitDeployment);
}

void NormalArmed::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = false;
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
    sfr::imu::turn_off = true;
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
    sfr::imu::turn_off = true;
}
void LowPowerArmed::dispatch()
{
    check_previous(sfr::mission::normalArmed, sfr::mission::transmitArmed);
}

void NormalInSun::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = false;
}
void NormalInSun::dispatch()
{
    timed_out(sfr::mission::transmitInSun, sfr::acs::on_time);
    enter_lp_insun();
    exit_insun_phase(sfr::mission::bootCamera);
}

void TransmitInSun::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::turn_off = true;
}
void TransmitInSun::dispatch()
{
    timed_out(sfr::mission::normalInSun, sfr::mission::acs_transmit_cycle_time - sfr::acs::on_time);
    enter_lp_insun();
    exit_insun_phase(sfr::mission::bootCamera);
}

void LowPowerInSun::transition_to()
{
    sfr::rockblock::sleep_mode = false;
    sfr::acs::off = true;
    sfr::imu::turn_off = true;
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
    sfr::imu::turn_off = true;
}

void VoltageFailureInSun::dispatch()
{
    if (sfr::battery::voltage_average->is_valid()) {
        sfr::mission::current_mode = sfr::mission::normalInSun;
    } else {
        exit_insun_phase(sfr::mission::bootCamera);
    }
}

void BootCamera::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::turn_off = true;
    sfr::camera::turn_on = true;
}
void BootCamera::dispatch()
{
    if (sfr::camera::init_mode == (uint16_t)camera_init_mode_type::complete || sfr::camera::failed_times > sfr::camera::failed_limit) {
        sfr::mission::current_mode = sfr::mission::mandatoryBurns;
    }
}

void MandatoryBurns::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::turn_off = true;
    sfr::mission::possible_uncovered = true;
}

void MandatoryBurns::dispatch()
{
    sfr::mission::current_mode = sfr::mission::regularBurns;
}

void RegularBurns::transition_to()
{
    sfr::rockblock::sleep_mode = true;
    sfr::acs::off = true;
    sfr::imu::turn_off = true;
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
    sfr::imu::turn_off = true;
    sfr::camera::take_photo = true;
}

void Photo::dispatch()
{
    sfr::mission::current_mode = sfr::mission::detumbleSpin;
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

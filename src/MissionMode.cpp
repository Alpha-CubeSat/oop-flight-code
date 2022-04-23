#include "MissionMode.hpp"
#include "sfr.hpp"

void Boot::transition_to() {}
void Boot::dispatch()
{
    timed_out(sfr::mission::aliveSignal, sfr::aliveSignal::max_time);
}


void AliveSignal::transition_to() {}
void AliveSignal::dispatch()
{
    enter_lp(sfr::mission::lowPowerAliveSignal);
    exit_signal_phase(sfr::mission::detumbleSpin);
}

void LowPowerAliveSignal::transition_to() {}
void LowPowerAliveSignal::dispatch() {
    exit_lp(sfr::mission::aliveSignal);
    exit_signal_phase(sfr::mission::lowPowerDetumbleSpin);
}

void DetumbleSpin::transition_to() {}
void DetumbleSpin::dispatch()
{
    enter_lp(sfr::mission::lowPowerDetumbleSpin);
    exit_detumble_phase(sfr::mission::normal);
}

void LowPowerDetumbleSpin::transition_to() {}
void LowPowerDetumbleSpin::dispatch()
{
    exit_lp(sfr::mission::detumbleSpin);
    exit_detumble_phase(sfr::mission::lowPower);
}

void Normal::transition_to() {
    sfr::mission::normal->set_start_time(millis());
}
void Normal::dispatch(){
    timed_out(sfr::mission::transmit, sfr::acs::on_time);
    enter_lp(sfr::mission::lowPower);
}

void LowPower::transition_to() {}
void LowPower::dispatch(){

}

void Transmit::transition_to() {}
void Transmit::dispatch(){

}

void NormalDeployment::transition_to() {}
void NormalDeployment::dispatch(){

}

void TransmitDeployment::transition_to() {}
void TransmitDeployment::dispatch(){

}

void LowPowerDeployment::transition_to() {}
void LowPowerDeployment::dispatch(){

}

void NormalArmed::transition_to() {}
void NormalArmed::dispatch(){

}

void TransmitArmed::transition_to() {}
void TransmitArmed::dispatch(){

}

void LowPowerArmed::transition_to() {}
void LowPowerArmed::dispatch(){

}

void NormalInSun::transition_to() {}
void NormalInSun::dispatch(){

}

void TransmitInSun::transition_to() {}
void TransmitInSun::dispatch(){

}

void LowPowerInSun::transition_to() {}
void LowPowerInSun::dispatch(){

}

void VoltageFailureInSun::transition_to() {}
void VoltageFailureInSun::dispatch(){

}

void BootCamera::transition_to() {}
void BootCamera::dispatch(){

}

void MandatoryBurns::transition_to() {}
void MandatoryBurns::dispatch(){

}

void RegularBurns::transition_to() {}
void RegularBurns::dispatch(){

}

void Photo::transition_to() {}
void Photo::dispatch(){

}

void exit_signal_phase(MissionMode *mode){
    if (sfr::aliveSignal::num_downlink_failures >= sfr::aliveSignal::max_downlink_failures || sfr::aliveSignal::downlinked) {
        sfr::mission::current_mode = mode;
    }   
}

void exit_detumble_phase(MissionMode *mode){
    if ((sfr::imu::gyro_z_average->is_valid() && sfr::imu::gyro_z_average->get_value() >= sfr::detumble::stable_gyro_z) || (millis()-sfr::detumble::start_time >= sfr::detumble::max_time)) {
        sfr::mission::current_mode = mode;
    }   
}

void enter_lp(MissionMode *lp_mode)
{
    if (!sfr::battery::voltage_average->is_valid() || sfr::battery::voltage_average->get_value() <= sfr::battery::min_battery) {
        sfr::mission::current_mode = lp_mode;
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




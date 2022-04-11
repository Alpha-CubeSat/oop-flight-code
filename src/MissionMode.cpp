#include "MissionMode.hpp"
#include "sfr.hpp"

void Boot::transition_to() {}
void Boot::dispatch()
{
    if (millis() - sfr::boot::start_time >= sfr::boot::max_time) {
        sfr::mission::current_mode = sfr::mission::aliveSignal;
    }
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
    sfr::normal::start_time = millis();
}
void Normal::dispatch(){

}

void LowPower::transition_to() {}
void LowPower::dispatch(){

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

/*void exit_acs()
{
    (millis()-sfr::normal::start_time >= sfr::mission::current_acs_mode->max_time() ){
        sfr::mission::current_mode = 
    }


}*/




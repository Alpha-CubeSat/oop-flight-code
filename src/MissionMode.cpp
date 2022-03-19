#include "MissionMode.hpp"
#include "sfr.hpp"

void Boot::transition_to() {}
void Boot::dispatch()
{
    if (millis() - sfr::mission::boot_start >= sfr::mission::max_boot_time) {
        sfr::mission::current_mode = sfr::mission::aliveSignal;
    }
}

void AliveSignal::transition_to() {}
void AliveSignal::dispatch()
{
    Serial.println("running this");
    check_entrance_lp(sfr::mission::lowPowerAliveSignal);
    if (sfr::rockblock::num_failures >= sfr::rockblock::max_failures) {
        Serial.println("ran this");
        sfr::mission::current_mode = sfr::mission::detumbleSpin;
    }
}

void LowPowerAliveSignal::transition_to()
{
    check_exit_lp(sfr::mission::aliveSignal);
}
void LowPowerAliveSignal::dispatch() {}

void DetumbleSpin::transition_to() {}
void DetumbleSpin::dispatch()
{
    check_entrance_lp(sfr::mission::lowPowerDetumbleSpin);
}

void LowPowerDetumbleSpin::transition_to() {}
void LowPowerDetumbleSpin::dispatch()
{
    check_exit_lp(sfr::mission::detumbleSpin);
}

void check_entrance_lp(MissionMode *lp_mode)
{
    Serial.println(sfr::battery::voltage_average->is_valid());
    Serial.println(sfr::battery::voltage_average->get_value() <= sfr::battery::min_battery);
    if (!sfr::battery::voltage_average->is_valid() || sfr::battery::voltage_average->get_value() <= sfr::battery::min_battery) {
        Serial.println("set to low power mode");
        sfr::mission::current_mode = lp_mode;
    }
}

void check_exit_lp(MissionMode *reg_mode)
{
    if (sfr::battery::voltage_average->is_valid() && sfr::battery::voltage_average->get_value() >= sfr::battery::acceptable_battery) {
        sfr::mission::current_mode = reg_mode;
    }
}

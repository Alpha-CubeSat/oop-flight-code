#include "BatteryMonitor.hpp"

BatteryMonitor::BatteryMonitor()
{
    pinMode(constants::battery::voltage_value_pin, OUTPUT);
}

void BatteryMonitor::execute()
{
    if (!initialized) {
        sfr::battery::voltage_value->set_valid();
        sfr::battery::voltage_average->set_valid();
        initialized = true;
    }
    float val = analogRead(constants::battery::voltage_value_pin) * constants::battery::voltage_ref / constants::battery::resolution;
    val = val * (constants::battery::r1 + constants::battery::r2) / constants::battery::r2;

    sfr::battery::voltage_average->set_value(val);
    sfr::battery::voltage_value->set_value(val);
}

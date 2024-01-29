#include "BatteryMonitor.hpp"

BatteryMonitor::BatteryMonitor()
{
}

void BatteryMonitor::execute()
{
    float val = analogRead(constants::battery::voltage_value_pin) * constants::sensor::voltage_ref / constants::sensor::resolution;
    val = val * (constants::battery::r1 + constants::battery::r2) / constants::battery::r2;

    sfr::battery::voltage_average->set_value(val);
    sfr::battery::voltage_value->set_value(val);
}

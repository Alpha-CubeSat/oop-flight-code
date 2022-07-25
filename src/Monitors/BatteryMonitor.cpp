#include "BatteryMonitor.hpp"

BatteryMonitor::BatteryMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    pinMode(constants::battery::voltage_value_pin, OUTPUT);
}

void BatteryMonitor::execute()
{
    float val = analogRead(constants::battery::voltage_value_pin) * constants::battery::voltage_ref / constants::battery::resolution;
    val = val * (constants::battery::r1 + constants::battery::r2) / constants::battery::r2;

    voltage_average->set_value(val);
    voltage_value->set_value(val);

#ifdef VERBOSE
    if (voltage_average->get_value(&val)) {
        Serial.print("Batt Voltage: ");
        Serial.print(val);
        Serial.println(" V");
    }
#endif
}

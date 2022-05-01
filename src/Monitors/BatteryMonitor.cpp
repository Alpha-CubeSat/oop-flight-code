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

    sfr::battery::voltage_buffer.push_front(val);
    sfr::battery::voltage = val;

    if (sfr::battery::voltage_buffer.size() > constants::sensor::collect) {
        sfr::battery::voltage_buffer.pop_back();
    }
    float sum = std::accumulate(sfr::battery::voltage_buffer.begin(), sfr::battery::voltage_buffer.end(), 0.0);
    sfr::battery::voltage_average->set_value(sum / sfr::battery::voltage_buffer.size());
#ifdef VERBOSE
    Serial.print("Batt Voltage: ");
    Serial.print(sfr::battery::voltage_average->get_value());
    Serial.println(" V");
#endif
}

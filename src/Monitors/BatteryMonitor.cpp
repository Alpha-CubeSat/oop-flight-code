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

    sfr::battery::voltage_average->set_value(val);
    sfr::battery::voltage_value->set_value(val);

#ifdef VERBOSE
    if (sfr::battery::voltage_average->get_value(&val)) {
        Serial.print("Batt Voltage: ");
        Serial.print(val);
        Serial.println(" V");
    }
#endif
    systime_duration_t duration = get_system_time() - TimedControlTaskBase::control_cycle_start_time;
    unsigned int dur = TimedControlTask::duration_to_us(duration);
    Serial.print("CT - BATTERY_MANAGER: ");
    sfr::timing::button_monitor_offset = dur;
    Serial.println(dur-sfr::timing::battery_monitor_offset);
}

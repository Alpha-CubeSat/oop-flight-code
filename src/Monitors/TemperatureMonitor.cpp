#include "TemperatureMonitor.hpp"

TemperatureMonitor::TemperatureMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    pinMode(constants::temperature::pin, INPUT);
}

void TemperatureMonitor::execute()
{
    float val = (((analogRead(constants::temperature::pin) * 3.3) / 1023) - .5) * 100;
    temp_c_average->set_value(val);
    temp_c_value->set_value(val);

    if (temp_c_average->get_value(&val)) {
#ifdef VERBOSE
        Serial.print("Temp: ");
        Serial.print(val);
        Serial.println(" C");
#endif
        sfr::temperature::in_sun = val >= constants::temperature::in_sun_val;
    } else {
        sfr::temperature::in_sun = false
    }
}
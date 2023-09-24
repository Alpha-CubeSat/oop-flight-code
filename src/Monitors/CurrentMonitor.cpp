#include "CurrentMonitor.hpp"

CurrentMonitor::CurrentMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    pinMode(constants::current::pin, OUTPUT);
}

void CurrentMonitor::execute()
{
    if (!initialized) {
        sfr::current::solar_current_average->set_valid();
        initialized = true;
    }

    uint16_t val = analogRead(constants::current::pin);
    float voltage = (val * constants::current::voltage_ref) / constants::current::resolution;
    float milliamps = 1000 * voltage / (constants::current::load * constants::current::shunt);

    sfr::current::solar_current_average->set_value(milliamps);

    if (sfr::current::solar_current_average->get_value(&milliamps)) {

#ifdef VERBOSE
        Serial.print("Current: ");
        Serial.print(milliamps);
        Serial.println(" mA");

#endif
        sfr::current::in_sun = milliamps >= constants::current::in_sun_val;
    } else {
        sfr::current::in_sun = false;
    }
}
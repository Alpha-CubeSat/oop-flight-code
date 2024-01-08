#include "CurrentMonitor.hpp"

CurrentMonitor::CurrentMonitor()
{
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
        sfr::current::in_sun = milliamps >= constants::current::in_sun_val;
    } else {
        sfr::current::in_sun = false;
    }
}
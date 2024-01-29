#include "TemperatureMonitor.hpp"

TemperatureMonitor::TemperatureMonitor()
{
}

void TemperatureMonitor::execute()
{
    float val = (((analogRead(constants::temperature::pin) * constants::sensor::voltage_ref) / constants::sensor::resolution) - 0.5) * 100;
    sfr::temperature::temp_c_average->set_value(val);
    sfr::temperature::temp_c_value->set_value(val);

    if (sfr::temperature::temp_c_average->get_value(&val)) {
        sfr::temperature::in_sun = val >= constants::temperature::in_sun_val;
    } else {
        sfr::temperature::in_sun = false;
    }
}
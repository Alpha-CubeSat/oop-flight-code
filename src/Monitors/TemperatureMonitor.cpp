#include "TemperatureMonitor.hpp"

TemperatureMonitor::TemperatureMonitor(unsigned int offset) : TimedControlTask<void>(offset)
{
    pinMode(constants::temperature::pin, INPUT);
}

void TemperatureMonitor::execute()
{
    float val = (((analogRead(constants::temperature::pin) * 3.3) / 1023) - .5) * 100;
    sfr::temperature::temp_c_buffer.push_front(val);
    sfr::temperature::temp_c = val;

    if (sfr::temperature::temp_c_buffer.size() > constants::sensor::collect) {
        sfr::temperature::temp_c_buffer.pop_back();
    }
    float sum = std::accumulate(sfr::temperature::temp_c_buffer.begin(), sfr::temperature::temp_c_buffer.end(), 0.0);
    sfr::temperature::temp_c_average = sum / sfr::temperature::temp_c_buffer.size();
#ifdef VERBOSE
    Serial.print("Temp: ");
    Serial.print(sfr::temperature::temp_c_average);
    Serial.println(" C");
#endif
    sfr::temperature::in_sun = sfr::temperature::temp_c_average >= constants::temperature::in_sun_val;
}
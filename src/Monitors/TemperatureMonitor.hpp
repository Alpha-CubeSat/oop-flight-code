#ifndef TEMPERATURE_MONITOR_HPP_
#define TEMPERATURE_MONITOR_HPP_

#include "sfr.hpp"

class TemperatureMonitor : public TimedControlTask<void>
{
public:
    TemperatureMonitor(unsigned int offset);
    void execute();

private:
    SensorReading *temp_c_average = new SensorReading(fault_index_type::temp_c, 20, sfr::temperature::temp_min, sfr::temperature::temp_max);
    SensorReading *temp_c_value = new SensorReading(1, sfr::temperature::temp_min, sfr::temperature::temp_max);
};

#endif
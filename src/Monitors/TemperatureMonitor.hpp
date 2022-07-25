#ifndef TEMPERATURE_MONITOR_HPP_
#define TEMPERATURE_MONITOR_HPP_

#include "SensorReading.hpp"
#include "sfr.hpp"

class TemperatureMonitor : public TimedControlTask<void>
{
public:
    TemperatureMonitor(unsigned int offset);
    void execute();

private:
    // LJG TODO work about temp min and max

    SensorReading *temp_c_average = new SensorReading(fault_index_type::temp_c, 20, -500, 500);
    SensorReading *temp_c_value = new SensorReading(1, -500, 500);
};

#endif
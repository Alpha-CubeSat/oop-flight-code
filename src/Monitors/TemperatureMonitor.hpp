#ifndef TEMPERATURE_MONITOR_HPP_
#define TEMPERATURE_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "SensorReading.hpp"

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
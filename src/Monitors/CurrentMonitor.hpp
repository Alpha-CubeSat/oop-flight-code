#ifndef CURRENT_MONITOR_HPP_
#define CURRENT_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "SensorReading.hpp"
#include "sfr.hpp"

class CurrentMonitor : public TimedControlTask<void>
{
public:
    CurrentMonitor(unsigned int offset);
    void execute();

private:
    // LJG TODO solar current min and max
    SensorReading *solar_current_average = new SensorReading(fault_index_type::solar_current, 20, 0, 1000);
};

#endif
#ifndef BATTERY_MONITOR_HPP_
#define BATTERY_MONITOR_HPP_

#include "SensorReading.hpp"
#include "sfr.hpp"

class BatteryMonitor : public TimedControlTask<void>
{
public:
    BatteryMonitor(unsigned int offset);
    void execute();

private:
    // LJG TODO determine min and max voltage
    SensorReading *voltage_value = new SensorReading(1, 0, 5);
    SensorReading *voltage_average = new SensorReading(fault_index_type::voltage, 20, 0, 5);
};

#endif
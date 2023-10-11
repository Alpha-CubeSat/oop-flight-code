#ifndef BATTERY_MONITOR_HPP_
#define BATTERY_MONITOR_HPP_

#include "sfr.hpp"

class BatteryMonitor
{
public:
    BatteryMonitor();
    void execute();

private:
    bool initialized = false;
};

#endif
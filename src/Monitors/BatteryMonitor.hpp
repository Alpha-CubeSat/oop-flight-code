#ifndef BATTERY_MONITOR_HPP_
#define BATTERY_MONITOR_HPP_

#include "sfr.hpp"
#include "MonitorsExecute.hpp"

class BatteryMonitor : public TimedControlTask<void>, public virtual MonitorsExecute
{
public:
    BatteryMonitor(unsigned int offset);
    void execute();
};

#endif
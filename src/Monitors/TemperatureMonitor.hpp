#ifndef TEMPERATURE_MONITOR_HPP_
#define TEMPERATURE_MONITOR_HPP_

#include "sfr.hpp"
#include "MonitorsExecute.hpp"

class TemperatureMonitor : public TimedControlTask<void>, public virtual MonitorsExecute
{
public:
    TemperatureMonitor(unsigned int offset);
    void execute();
};

#endif
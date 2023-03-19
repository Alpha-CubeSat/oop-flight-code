#ifndef TEMPERATURE_MONITOR_HPP_
#define TEMPERATURE_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "constants.hpp"
#include "sfr.hpp"

class TemperatureMonitor : public TimedControlTask<void>
{
public:
    TemperatureMonitor(unsigned int offset);
    void execute();

private:
    bool initialized = false;
};

#endif
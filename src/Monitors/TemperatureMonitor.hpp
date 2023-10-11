#ifndef TEMPERATURE_MONITOR_HPP_
#define TEMPERATURE_MONITOR_HPP_

#include "constants.hpp"
#include "sfr.hpp"

class TemperatureMonitor 
{
public:
    TemperatureMonitor();
    void execute();

private:
    bool initialized = false;
};

#endif
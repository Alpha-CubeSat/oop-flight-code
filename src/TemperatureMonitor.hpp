#ifndef TEMPERATURE_MONITOR_HPP_
#define TEMPERATURE_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <Arduino.h> 

class TemperatureMonitor{
    public:
        TemperatureMonitor();
        void execute();
};

#endif
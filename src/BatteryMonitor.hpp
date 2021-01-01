#ifndef BATTERY_MONITOR_HPP_
#define BATTERY_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <Arduino.h>

class BatteryMonitor{
    public:
        BatteryMonitor();
        void execute();
};

#endif
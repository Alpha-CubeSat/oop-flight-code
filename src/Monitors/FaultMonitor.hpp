#ifndef FAULT_MONITOR_HPP_
#define FAULT_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <Arduino.h>

class FaultMonitor{
    public:
        FaultMonitor();
        void execute();
};

#endif
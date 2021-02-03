#ifndef CURRENT_MONITOR_HPP_
#define CURRENT_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <Arduino.h> 

class CurrentMonitor{
    public:
        CurrentMonitor();
        void execute();
};

#endif
#ifndef BUTTON_MONITOR_HPP_
#define BUTTON_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <Arduino.h>

class ButtonMonitor{
    public:
        ButtonMonitor();
        void execute();
};

#endif
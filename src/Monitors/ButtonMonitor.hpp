#ifndef BUTTON_MONITOR_HPP_
#define BUTTON_MONITOR_HPP_

#include "constants.hpp"
#include "sfr.hpp"

class ButtonMonitor 
{
public:
    ButtonMonitor();
    void execute();

private:
    float val;
};

#endif
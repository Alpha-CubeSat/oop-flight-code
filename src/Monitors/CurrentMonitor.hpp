#ifndef CURRENT_MONITOR_HPP_
#define CURRENT_MONITOR_HPP_

#include "sfr.hpp"

class CurrentMonitor 
{
public:
    CurrentMonitor();
    void execute();

private:
    bool initialized = false;
};

#endif
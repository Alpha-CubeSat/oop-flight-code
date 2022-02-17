#ifndef FAULT_MONITOR_HPP_
#define FAULT_MONITOR_HPP_

#include "sfr.hpp"

class FaultMonitor : public TimedControlTask<void>
{
public:
    FaultMonitor(unsigned int offset);
    void execute();
};

#endif
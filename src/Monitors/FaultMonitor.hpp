#ifndef FAULT_MONITOR_HPP_
#define FAULT_MONITOR_HPP_

#include "sfr.hpp"
#include "MonitorsExecute.hpp"

class FaultMonitor : public TimedControlTask<void>, public virtual MonitorsExecute
{
public:
    FaultMonitor(unsigned int offset);
    void execute();
};

#endif
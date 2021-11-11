#ifndef CURRENT_MONITOR_HPP_
#define CURRENT_MONITOR_HPP_

#include "sfr.hpp"
#include "MonitorsExecute.hpp"

class CurrentMonitor : public TimedControlTask<void>, public virtual MonitorsExecute
{
public:
    CurrentMonitor(unsigned int offset);
    void execute();
};

#endif
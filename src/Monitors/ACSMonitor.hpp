#ifndef ACS_MONITOR_HPP_
#define ACS_MONITOR_HPP_

#include "sfr.hpp"
#include "MonitorsExecute.hpp"

class ACSMonitor : public TimedControlTask<void>, public virtual MonitorsExecute
{
public:
    ACSMonitor(unsigned int offset);
    void execute();
    StarshotACS0ModelClass rtObj;
};

#endif
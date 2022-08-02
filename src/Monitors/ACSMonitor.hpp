#ifndef ACS_MONITOR_HPP_
#define ACS_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"

class ACSMonitor : public TimedControlTask<void>
{
public:
    ACSMonitor(unsigned int offset);
    void execute();
};

#endif
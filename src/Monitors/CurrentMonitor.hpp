#ifndef CURRENT_MONITOR_HPP_
#define CURRENT_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "sfr.hpp"

class CurrentMonitor : public TimedControlTask<void>
{
public:
    CurrentMonitor(unsigned int offset);
    void execute();

private:
    bool initialized = false;
};

#endif
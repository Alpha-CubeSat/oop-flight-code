#ifndef COMMAND_MONITOR_HPP_
#define COMMAND_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "sfr.hpp"

class CommandMonitor : public TimedControlTask<void>
{
public:
    CommandMonitor(unsigned int offset);
    void execute();
};

#endif
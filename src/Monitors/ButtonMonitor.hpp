#ifndef BUTTON_MONITOR_HPP_
#define BUTTON_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "constants.hpp"
#include "sfr.hpp"

class ButtonMonitor : public TimedControlTask<void>
{
public:
    ButtonMonitor(unsigned int offset);
    void execute();
};

#endif
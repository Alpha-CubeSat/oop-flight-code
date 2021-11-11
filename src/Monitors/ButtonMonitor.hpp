#ifndef BUTTON_MONITOR_HPP_
#define BUTTON_MONITOR_HPP_

#include "sfr.hpp"
#include "MonitorsExecute.hpp"

class ButtonMonitor : public TimedControlTask<void>, public virtual MonitorsExecute
{
public:
    ButtonMonitor(unsigned int offset);
    void execute();
};

#endif
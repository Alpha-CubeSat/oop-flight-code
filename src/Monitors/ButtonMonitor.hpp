#ifndef BUTTON_MONITOR_HPP_
#define BUTTON_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include "Control Tasks/TimedControlTask.hpp"

class ButtonMonitor : public TimedControlTask<void>{
    public:
        ButtonMonitor(unsigned int offset);
        void execute();
};

#endif
#ifndef ACS_MONITOR_HPP_
#define ACS_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <StarshotACS0.h>
#include "Control Tasks/TimedControlTask.hpp"

class ACSMonitor : public TimedControlTask<void>{
    public:
        ACSMonitor(unsigned int offset);
        void execute();
        StarshotACS0ModelClass rtObj;
};

#endif
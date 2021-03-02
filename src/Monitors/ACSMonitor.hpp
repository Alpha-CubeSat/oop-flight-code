#ifndef ACS_MONITOR_HPP_
#define ACS_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <StarshotACS0.h>

class ACSMonitor{
    public:
        ACSMonitor();
        void execute();
        StarshotACS0ModelClass rtObj;
};

#endif
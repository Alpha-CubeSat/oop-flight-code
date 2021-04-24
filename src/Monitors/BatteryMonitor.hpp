#ifndef BATTERY_MONITOR_HPP_
#define BATTERY_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include "Control Tasks/TimedControlTask.hpp"

class BatteryMonitor : public TimedControlTask<void>{
    public:
        BatteryMonitor(unsigned int offset);
        void execute();
};

#endif
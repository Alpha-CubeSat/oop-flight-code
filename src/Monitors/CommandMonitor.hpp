#ifndef COMMAND_MONITOR_HPP_
#define COMMAND_MONITOR_HPP_

#include "sfr.hpp"
#include "Control Tasks/TimedControlTask.hpp"

class CommandMonitor : public TimedControlTask<void>{
    public:
        CommandMonitor(unsigned int offset);
        void execute();
        void dispatch_change_mission_mode();
        void dispatch_change_fault_checks();
};

#endif
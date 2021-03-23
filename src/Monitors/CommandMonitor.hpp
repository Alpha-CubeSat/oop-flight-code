#ifndef COMMAND_MONITOR_HPP_
#define COMMAND_MONITOR_HPP_

#include "sfr.hpp"

class CommandMonitor{
    public:
        CommandMonitor();
        void execute();
        void dispatch_change_mission_mode();
        void dispatch_change_fault_checks();
};

#endif
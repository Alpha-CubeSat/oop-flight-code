#ifndef MAIN_CONTROL_LOOP_HPP_
#define MAIN_CONTROL_LOOP_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include "RockblockControlTask.hpp"
#include "IMUMonitor.hpp"

class MainControlLoop{
    protected:
        RockblockControlTask rockblock_control_task;
        IMUMonitor imu_monitor;
    
    public:
        MainControlLoop();
        void execute();
};

#endif
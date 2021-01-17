#ifndef MAIN_CONTROL_LOOP_HPP_
#define MAIN_CONTROL_LOOP_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include "RockblockControlTask.hpp"
#include "IMUMonitor.hpp"
#include "CameraControlTask.hpp"

class MainControlLoop{
    protected:
        //RockblockControlTask rockblock_control_task;
        //IMUMonitor imu_monitor;
        CameraControlTask camera_control_task;
    
    public:
        MainControlLoop();
        void execute();
};

#endif
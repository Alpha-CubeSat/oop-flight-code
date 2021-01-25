#ifndef MAIN_CONTROL_LOOP_HPP_
#define MAIN_CONTROL_LOOP_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include "ACSControlTask.hpp"
#include "ACSMonitor.hpp"
#include "BatteryMonitor.hpp"
#include "BurnwireControlTask.hpp"
#include "CameraControlTask.hpp"
#include "CurrentMonitor.hpp"
#include "FaultMonitor.hpp"
#include "IMUMonitor.hpp"
#include "MissionManager.hpp"
#include "PhotoresistorMonitor.hpp"
#include "RockblockControlTask.hpp"
#include "TemperatureMonitor.hpp"

class MainControlLoop{
    protected:
        ACSMonitor acs_monitor;
        BatteryMonitor battery_monitor;
        CurrentMonitor current_monitor;
        IMUMonitor imu_monitor;
        PhotoresistorMonitor photoresistor_monitor;
        TemperatureMonitor temperature_monitor;

        FaultMonitor fault_monitor;

        MissionManager mission_manager;

        ACSControlTask acs_control_task;
        BurnwireControlTask burnwire_control_task;
        CameraControlTask camera_control_task;
        RockblockControlTask rockblock_control_task;
    
    public:
        MainControlLoop();
        void execute();
};

#endif
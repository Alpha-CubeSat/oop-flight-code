#ifndef MAIN_CONTROL_LOOP_HPP_
#define MAIN_CONTROL_LOOP_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include "Control Tasks/ACSControlTask.hpp"
#include "Monitors/ACSMonitor.hpp"
#include "Monitors/BatteryMonitor.hpp"
#include "Monitors/ButtonMonitor.hpp"
#include "Control Tasks/BurnwireControlTask.hpp"
#include "Control Tasks/CameraControlTask.hpp"
#include "Monitors/CurrentMonitor.hpp"
#include "Monitors/FaultMonitor.hpp"
#include "Monitors/IMUMonitor.hpp"
#include "MissionManager.hpp"
#include "Monitors/PhotoresistorMonitor.hpp"
#include "Control Tasks/RockblockControlTask.hpp"
#include "Monitors/TemperatureMonitor.hpp"

class MainControlLoop{
    protected:
        ACSMonitor acs_monitor;
        BatteryMonitor battery_monitor;
        ButtonMonitor button_monitor;
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
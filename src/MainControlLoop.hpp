#ifndef MAIN_CONTROL_LOOP_HPP_
#define MAIN_CONTROL_LOOP_HPP_

#include "ClockManager.hpp"
#include "Control Tasks/ACSControlTask.hpp"
#include "Control Tasks/BurnwireControlTask.hpp"
#include "Control Tasks/CameraControlTask.hpp"
#include "Control Tasks/RockblockControlTask.hpp"
#include "Monitors/ACSMonitor.hpp"
#include "Monitors/BatteryMonitor.hpp"
#include "Monitors/ButtonMonitor.hpp"
#include "Monitors/CameraReportMonitor.hpp"
#include "Monitors/CommandMonitor.hpp"
#include "Monitors/CurrentMonitor.hpp"
#include "Monitors/FaultMonitor.hpp"
#include "Monitors/IMUDownlinkReportMonitor.hpp"
#include "Monitors/NormalReportMonitor.hpp"
#include "Monitors/PhotoresistorMonitor.hpp"
#include "Monitors/RockblockReportMonitor.hpp"
#include "Monitors/TemperatureMonitor.hpp"
#include "sfr.hpp"

class MainControlLoop : ControlTask<void>
{
protected:
    ClockManager clock_manager;

    ACSMonitor acs_monitor;
    BatteryMonitor battery_monitor;
    ButtonMonitor button_monitor;
    CameraReportMonitor camera_report_monitor;
    CommandMonitor command_monitor;
    CurrentMonitor current_monitor;
    FaultMonitor fault_monitor;
    IMUDownlinkReportMonitor imu_monitor;
    NormalReportMonitor normal_report_monitor;
    PhotoresistorMonitor photoresistor_monitor;
    RockblockReportMonitor rockblock_report_monitor;
    TemperatureMonitor temperature_monitor;

    ACSControlTask acs_control_task;
    BurnwireControlTask burnwire_control_task;
    CameraControlTask camera_control_task;
    RockblockControlTask rockblock_control_task;

    MissionManager mission_manager;

public:
    MainControlLoop();
    void execute() override;
};

#endif
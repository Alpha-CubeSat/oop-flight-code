#ifndef MAIN_CONTROL_LOOP_HPP_
#define MAIN_CONTROL_LOOP_HPP_

#include "ClockManager.hpp"
#include "Control Tasks/BurnwireControlTask.hpp"
#include "Control Tasks/CameraControlTask.hpp"
#include "Control Tasks/EEPROMControlTask.hpp"
#include "Control Tasks/RockblockControlTask.hpp"
#include "Faults.hpp"
#include "MissionManager.hpp"
#include "Monitors/BatteryMonitor.hpp"
#include "Monitors/ButtonMonitor.hpp"
#include "Monitors/CameraReportMonitor.hpp"
#include "Monitors/CommandMonitor.hpp"
#include "Monitors/CurrentMonitor.hpp"
#include "Monitors/IMUDownlink.hpp"
#include "Monitors/IMUDownlinkReportMonitor.hpp"
#include "Monitors/IMUMonitor.hpp"
#include "Monitors/NormalReportMonitor.hpp"
#include "Monitors/PhotoresistorMonitor.hpp"
#include "Monitors/RockblockReportMonitor.hpp"
#include "Monitors/TemperatureMonitor.hpp"

class MainControlLoop : ControlTask<void>
{
protected:
    ClockManager clock_manager;

    BatteryMonitor battery_monitor;
    ButtonMonitor button_monitor;
    CameraReportMonitor camera_report_monitor;
    CommandMonitor command_monitor;
    CurrentMonitor current_monitor;
    IMUMonitor imu_monitor;
    IMUDownlink imu_downlink;
    NormalReportMonitor normal_report_monitor;
    IMUDownlinkReportMonitor imudownlink_report_monitor;
    PhotoresistorMonitor photoresistor_monitor;
    RockblockReportMonitor rockblock_report_monitor;
    TemperatureMonitor temperature_monitor;

    BurnwireControlTask burnwire_control_task;
    CameraControlTask camera_control_task;
    RockblockControlTask rockblock_control_task;
    EEPROMControlTask eeprom_control_task;

    MissionManager mission_manager;

public:
    MainControlLoop();
    void execute() override;
};

#endif
#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop()
: ControlTask<void>(),
clock_manager(constants::timecontrol::control_cycle_time), 
acs_monitor(constants::timecontrol::acs_monitor_offset),
battery_monitor(constants::timecontrol::battery_monitor_offset),
button_monitor(constants::timecontrol::button_monitor_offset),
current_monitor(constants::timecontrol::current_monitor_offset),
imu_monitor(constants::timecontrol::imu_monitor_offset),
temperature_monitor(constants::timecontrol::temperature_monitor_offset),
camera_report_monitor(constants::timecontrol::camera_report_monitor_offset),
photoresistor_monitor(constants::timecontrol::photoresistor_monitor_offset),
fault_monitor(constants::timecontrol::fault_monitor_offset),
mission_manager(constants::timecontrol::mission_manager_offset),
acs_control_task(constants::timecontrol::acs_control_task_offset),
burnwire_control_task(constants::timecontrol::burnwire_control_task_offset),
camera_control_task(constants::timecontrol::camera_control_task_offset),
rockblock_control_task(constants::timecontrol::rockblock_control_task_offset)
{
    delay(1000);
}

void MainControlLoop::execute()
{

    /*clock_manager.execute();
    //Serial.println("CLOCK MANAGER EXECUTED")

    acs_monitor.execute_on_time();
    //Serial.println("ACS MONITOR EXECUTED");
    battery_monitor.execute_on_time();
    //Serial.println("BATTERY MONITOR EXECUTED");
    button_monitor.execute_on_time();
    //Serial.println("BUTTON MONITOR EXECUTED");
    current_monitor.execute_on_time();
    //Serial.println("CURRENT MONITOR EXECUTED");
    imu_monitor.execute_on_time();
    //Serial.println(micros());
    //Serial.println("IMU MONITOR EXECUTED");
    photoresistor_monitor.execute_on_time();
    //Serial.println("PHOTORESISTOR MONITOR EXECUTED");
    temperature_monitor.execute_on_time();
    //Serial.println("TEMPERATURE MONITOR EXECUTED");
    //camera_report_monitor.execute_on_time();
    //Serial.println("CAMERA REPORT MONITOR EXECUTED");
    fault_monitor.execute_on_time();
    //Serial.println("FAULT MONITOR EXECUTED");

    mission_manager.execute_on_time();
    //Serial.println("MISSION MANAGER EXECUTED");

    acs_control_task.execute_on_time();
    //Serial.println("ACS CONTROL TASK EXECUTED");
    burnwire_control_task.execute_on_time();
    //Serial.println("BURNWIRE CONTROL TASK EXECUTED");
    //camera_control_task.execute_on_time();
    //Serial.println("CAMERA CONTROL TASK EXECUTED");
    rockblock_control_task.execute_on_time();
    //Serial.println("ROCKBLOCK CONTROL EXECUTED");*/

    rockblock_control_task.execute();
}
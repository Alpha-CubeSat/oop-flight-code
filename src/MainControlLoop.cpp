#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop():
    acs_monitor(),
    battery_monitor(),
    button_monitor(),
    current_monitor(),
    imu_monitor(),
    photoresistor_monitor(),
    temperature_monitor(),
        
    fault_monitor(),

    acs_control_task(),
    burnwire_control_task(),
    camera_control_task(),
    rockblock_control_task(){
        delay(1000);
    }

void MainControlLoop::execute(){
    
    acs_monitor.execute();
    Serial.println("ACS MONITOR EXECUTED");
    battery_monitor.execute();
    Serial.println("BATTERY MONITOR EXECUTED");
    button_monitor.execute();
    Serial.println("BUTTON MONITOR EXECUTED");
    current_monitor.execute();
    Serial.println("CURRENT MONITOR EXECUTED");
    imu_monitor.execute();
    Serial.println("IMU MONITOR EXECUTED");
    photoresistor_monitor.execute();
    Serial.println("PHOTORESISTOR MONITOR EXECUTED");
    temperature_monitor.execute();
    Serial.println("TEMPERATURE MONITOR EXECUTED");
        
    fault_monitor.execute();
    Serial.println("FAULT MONITOR EXECUTED");

    mission_manager.execute();
    Serial.println("MISSION MANAGER EXECUTED");

    acs_control_task.execute();
    Serial.println("ACS CONTROL TASK EXECUTED");
    burnwire_control_task.execute();
    Serial.println("BURNWIRE CONTROL TASK EXECUTED");
    camera_control_task.execute();
    Serial.println("CAMERA CONTROL TASK EXECUTED");
    rockblock_control_task.execute();
    Serial.println("ROCKBLOCK CONTROL EXECUTED");
    
}
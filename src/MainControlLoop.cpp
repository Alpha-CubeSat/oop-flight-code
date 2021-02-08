#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop(){
    delay(1000);
}

void MainControlLoop::execute(){
    //start_time = millis();
    //pass in to execute
    
    acs_monitor.execute();
    //Serial.println("ACS MONITOR EXECUTED");
    battery_monitor.execute();
    //Serial.println("BATTERY MONITOR EXECUTED");
    button_monitor.execute();
    //Serial.println("BUTTON MONITOR EXECUTED");
    current_monitor.execute();
    //Serial.println("CURRENT MONITOR EXECUTED");
    imu_monitor.execute();
    //Serial.println("IMU MONITOR EXECUTED");
    photoresistor_monitor.execute();
    //Serial.println("PHOTORESISTOR MONITOR EXECUTED");
    temperature_monitor.execute();
    //Serial.println("TEMPERATURE MONITOR EXECUTED");
        
    fault_monitor.execute();
    //Serial.println("FAULT MONITOR EXECUTED");

    mission_manager.execute();
    //Serial.println("MISSION MANAGER EXECUTED");

    acs_control_task.execute();
    //Serial.println("ACS CONTROL TASK EXECUTED");
    burnwire_control_task.execute();
    //Serial.println("BURNWIRE CONTROL TASK EXECUTED");
    camera_control_task.execute();
    //Serial.println("CAMERA CONTROL TASK EXECUTED");
    rockblock_control_task.execute();
    //Serial.println("ROCKBLOCK CONTROL EXECUTED");
    
}
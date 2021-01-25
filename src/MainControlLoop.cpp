#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop():
    acs_monitor(),
    battery_monitor(),
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
    Serial.println("1");
    battery_monitor.execute();
    Serial.println("2");
    current_monitor.execute();
    Serial.println("3");
    imu_monitor.execute();
    Serial.println("4");
    photoresistor_monitor.execute();
    Serial.println("5");
    temperature_monitor.execute();
    Serial.println("6");
        
    fault_monitor.execute();
    Serial.println("7");

    mission_manager.execute();
    Serial.println("8");

    acs_control_task.execute();
    Serial.println("9");
    burnwire_control_task.execute();
    Serial.println("10");
    camera_control_task.execute();
    Serial.println("11");
    rockblock_control_task.execute();
    Serial.println("12");
    
}
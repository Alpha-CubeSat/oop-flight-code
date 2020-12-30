#include "MainControlLoop.hpp"

/*MainControlLoop::MainControlLoop():
    gps_monitor(Serial3),
    photoresistor_monitor(),
    uplink_monitor(),
    mission_manager(),
    burnwire_control_task(),
    camera_control_task(){
        delay(1000);
}*/

MainControlLoop::MainControlLoop():
    photoresistor_monitor(), 
    burnwire_control_task(),
    camera_control_task(){
        delay(1000);
}

void MainControlLoop::execute(){
    photoresistor_monitor.execute();

    //mission_manager.execute();

    burnwire_control_task.execute();
    camera_control_task.execute();
}
#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop():
    rockblock_control_task(),
    imu_monitor(){
        delay(1000);
}

void MainControlLoop::execute(){
    imu_monitor.execute();
    rockblock_control_task.execute();
}
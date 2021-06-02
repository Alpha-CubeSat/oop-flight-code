#include "MainControlLoop.hpp"
#include "Control Tasks/PinControlTask.hpp"

MainControlLoop mcl;
PinControlTask pin_control_task;

#ifndef UNIT_TEST
void setup(){
    //delay for 2 hours
    //delay(7200000);
    pin_control_task.execute();  
}

void loop(){
    mcl.execute();
}
#endif




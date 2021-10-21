#include "MainControlLoop.hpp"
#include "Control Tasks/PinControlTask.hpp"

MainControlLoop mcl;
PinControlTask pin_control_task;

#ifndef UNIT_TEST
void setup(){
    //delay for 2 hours
    //delay(7200000);
    //pin_control_task.execute();  
    pinMode(14,OUTPUT);
    digitalWrite(14,LOW);
    delay(2000);
    digitalWrite(14,HIGH);
    delay(500);
    digitalWrite(14,LOW);
}

void loop(){
    //mcl.execute();
}
#endif




#include "Control Tasks/PinControlTask.hpp"
#include "MainControlLoop.hpp"

MainControlLoop mcl;
PinControlTask pin_control_task;

#ifndef UNIT_TEST

void setup()
{
    // delay for 2 hours
    // delay(7200000);
    Serial.begin(9600);
    delay(5000);
    pin_control_task.execute();
}

void loop()
{
    mcl.execute();
}
#endif

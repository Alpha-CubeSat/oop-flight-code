#include "MainControlLoop.hpp"
#include "Pins.hpp"

MainControlLoop mcl;

#ifndef UNIT_TEST

void setup()
{
    // delay for 2 hours
    // delay(7200000);
    Serial.begin(9600);
    delay(5000);
    Pins::setInitialPinStates();
}

void loop()
{
    mcl.execute();
}
#endif
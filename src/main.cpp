#include "EEPROMRestore.hpp"
#include "MainControlLoop.hpp"
#include "Pins.hpp"

MainControlLoop mcl;

#ifndef UNIT_TEST

void setup()
{
    Pins::setInitialPinStates();
    Serial.begin(9600);
    EEPROMRestore::execute(); //TODO REVIEW
}

void loop()
{
    mcl.execute();
}

#endif
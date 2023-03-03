#include "EEPROMRestore.hpp"
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
    // TODO FS-175: test and uncomment line below
    // EEPROMRestore::execute();
}

void loop()
{
    // Uncomment the next line to do IMU Downlink test for sail deployment
    sfr::mission::current_mode = sfr::mission::mandatoryBurns;
    mcl.execute();
}
#endif
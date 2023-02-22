#include "EEPROMRestore.hpp"
#include "MainControlLoop.hpp"
#include "Pins.hpp"

MainControlLoop mcl;

#ifndef UNIT_TEST

int start = 1;
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
    if (start) {
        sfr::imu::powered = true; // Assuming initialization went correctly
        sfr::mission::current_mode = sfr::mission::bootCamera;
    }
    mcl.execute();
}
#endif
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
    // TODO: Uncomment when EEPROMRestore is fixed (FS-186)
    // EEPROMRestore::execute();
}

void loop()
{
    // Uncomment the next line to do IMU Downlink test for sail deployment
    sfr::mission::current_mode = sfr::mission::mandatoryBurns;
    int start = 1;
    if (start) {
        sfr::imu::powered = true; // Assuming initialization went correctly
        sfr::mission::current_mode = sfr::mission::bootCamera;
    }
    mcl.execute();
}
#endif
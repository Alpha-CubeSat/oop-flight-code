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
    // int start = 1;
    // if (start) {
    //     sfr::imu::turn_on == true && sfr::imu::powered == false;
    //     sfr::mission::current_mode = sfr::mission::bootCamera;
    // }
    mcl.execute();
}
#endif
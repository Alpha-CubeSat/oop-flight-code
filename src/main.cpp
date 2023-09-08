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
    EEPROMRestore::execute();

#ifdef NO_ACS
    sfr::acs::on_time = 0;
    sfr::acs::detumble_timeout = 0;
#endif
}

void loop()
{
    mcl.execute();
}
#endif
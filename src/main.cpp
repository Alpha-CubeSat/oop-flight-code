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
    Serial.println(EEPROM.length());
    int eeprom_length = EEPROM.length();
    for (int i = 0; i < eeprom_length; i++) {
        EEPROM.write(i, 0);
    }

    EEPROM.write(4, 1);
    // TODO FS-175: test and uncomment line below
    EEPROMRestore::execute();
    uint8_t x;
    EEPROM.get(4, x);
    Serial.println(x);
}

void loop()
{
    mcl.execute();
}
#endif
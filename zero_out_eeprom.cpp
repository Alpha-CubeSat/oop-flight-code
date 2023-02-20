#include <Arduino.h>
#include <EEPROM.h>

// Script for zeroing out all EERPOM bytes

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.println("Beginning EEPROM clear!");
    int eeprom_length = EEPROM.length();
    for (int i = 0; i < eeprom_length; i++) {
        EEPROM.write(i, 0);
    }
    Serial.println("Clear finished, will clear again in 5 seconds. You can unplug to save write endurance!");
    delay(5000);
}
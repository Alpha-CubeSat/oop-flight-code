#include <Arduino.h>
#include <EEPROM.h>

// Script for resetting all EERPOM bytes
// Set PlatformIO environment to `env:reset_eeprom` then upload.

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.println("Beginning EEPROM reset!");
    int eeprom_length = EEPROM.length();
    for (int i = 0; i < eeprom_length; i++) {
        EEPROM.write(i, 0);
    }

    int eeprom_length = EEPROM.length();
    for (int i = 0; i < eeprom_length; i++) {
        EEPROM.write(i, 0);
    }

    // EEPROM.put(0, (uint32_t)0);
    // EEPROM.put(4086, (uint32_t)0);
    // EEPROM.put(4, (uint8_t)0);
    // EEPROM.put(4090, (uint8_t)0);
    EEPROM.put(5, (bool)false);
    EEPROM.put(4091, (bool)false);
    EEPROM.put(6, (uint16_t)10);
    EEPROM.put(4092, (uint16_t)10);
    EEPROM.put(8, (uint16_t)460);
    EEPROM.put(4094, (uint16_t)460);
    Serial.println("Reset finished, will run again in 5 seconds. You can unplug to save write endurance!");
    delay(5000);
}
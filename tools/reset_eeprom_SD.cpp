#include <Arduino.h>
#include <EEPROM.h>
#include <SD.h>

#define NUM_FILES 10

bool create_files = false;

// Script for resetting all EERPOM bytes and clearing the SD card
// Set PlatformIO environment to `env:reset_eeprom` then upload.

void setup()
{
    Serial.begin(9600);

    if (!SD.begin(254)) {
        while(true){
            Serial.println("Error: Failed to init SD card");
        }
    }

    // Create files if specified
    if (create_files) {

        for (int i = 0; i < NUM_FILES; i++) {
            String filename = "file" + String(i) + ".txt";
            File file = SD.open(filename.c_str(), FILE_WRITE);
            Serial.print("Creating ");
            Serial.println(filename.c_str());
            file.close();
        }
    }

    // Delete files
    File root = SD.open("/");

    if (root) {
        while (true) {
            File entry = root.openNextFile();
            Serial.print("Deleting ");
            Serial.println(entry.name());

            if (!entry) {
                break;
            }

            if (!SD.remove(entry.name())) {
                Serial.print("Deleting ");
                Serial.print(entry.name());
                Serial.println(" unsuccessful");
            }
        }
        root.close();
    } else {
        Serial.println("Error: Opening root failed");
    }
    Serial.println("All files deleted");

    Serial.println("Beginning EEPROM reset!");
    uint16_t eeprom_length = EEPROM.length();
    for (unsigned int i = 0; i < eeprom_length; i++) {
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
    Serial.println("EEPROM reset finished!");  
}

void loop()
{
    Serial.println("SD and EEPROM reset finished!");
    delay(10000);
}
#include <Arduino.h>
#include <SD.h>
#include <string>

#define NUM_FILES 10

bool create_files = false;

// Script for deleting all files from the SD card
// Can specify to create dummy files to test functionality if needed by
// setting the create_files flag to true.

// Set PlatformIO environment to `env:clear_sd_card` then upload.

void setup()
{
    Serial.begin(9600);

    if (!SD.begin(254)) {
        Serial.println("Error: Failed to init SD card");
        return;
    }

    // Create files if specified
    if (create_files) {

        for (int i = 0; i < NUM_FILES; i++) {
            std::string filename = "file" + std::to_string(i) + ".txt";
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
            // entry.close();

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
}

void loop()
{
}
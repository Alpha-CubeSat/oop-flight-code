#include <Arduino.h>
#include <SD.h>

constexpr uint8_t camera_fragment_length = 64;

void setup()
{
    Serial.begin(9600);

    if (!SD.begin(254)) {
        Serial.println("Error: Failed to init SD card");
        return;
    }

    File root = SD.open("/");

    if (root) {
        while (true) {
            // assumed that this gives files in alphabetical order. not sure if this is true though?
            File entry = root.openNextFile();

            if (!entry) {
                Serial.println("Error: Opening file failed");
                break;
            }

            String filename = entry.name();
            Serial.println(filename);

            // fragment image filenames should be 8 chars long
            if (filename.endsWith(".jpg") && filename.length() == 8) {
                
                // assumed that serial and fragment # are both 2 digits
                String curSerial = filename.substring(0, 2);
                String curFragment = filename.substring(2, 4);
                File imgFile = SD.open(("/" + curSerial).c_str(), FILE_WRITE);
                Serial.println("Opened image serial #" + curSerial);

                // read fragment file
                uint8_t bytesToRead = min(camera_fragment_length, entry.size());
                uint8_t *fragmentBuffer;
                entry.read(fragmentBuffer, bytesToRead);
                Serial.println("Read fragment #" + curFragment);

                // append fragemnt to end of image file
                imgFile.write(fragmentBuffer, bytesToRead);

                Serial.println("Appended fragament #" + curFragment + "to image serial #" + curSerial);
            }
        }
        root.close();
    } else {
        Serial.println("Error: Opening root failed");
    }
    Serial.println("Done!");
}

void loop()
{

}
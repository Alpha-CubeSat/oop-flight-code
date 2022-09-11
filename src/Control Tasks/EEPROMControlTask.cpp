#include "EEPROMControlTask.hpp"

EEPROMControlTask::EEPROMControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void EEPROMControlTask::execute()
{
    if (!sfr::EEPROM::alloted_time_passed) {
        // The wait time of two hours has not been reached since last read
        // the amount of time that the EEPROM has counted, stops when the alloted time has been reached
        int byte0 = EEPROM.read(0) << 24;
        int byte1 = EEPROM.read(1) << 16;
        int byte2 = EEPROM.read(2) << 8;
        int byte3 = EEPROM.read(3);
        int eeprom_value = byte0 + byte1 + byte2 + byte3;

        Serial.println("EEPROM Value: " + eeprom_value);
        if (eeprom_value > sfr::EEPROM::alloted_time) {
            // The newly read eeprom value exceeds the two hour wait time
            sfr::EEPROM::alloted_time_passed = true;
            Serial.println("EEPROM Time tracking finished");
        } else {
            int time_since_last_write = millis() - sfr::EEPROM::time_of_last_write;

            if (time_since_last_write > sfr::EEPROM::write_step_time) {
                // The last EEPROM write exceeds the interval between writes, so update the EEPROM value
                int write = time_since_last_write + eeprom_value;
                Serial.println("New value to write: " + write);

                // Saves the value in write by bytes
                EEPROM.write(3, write & 0xFF);
                EEPROM.write(2, (write >> 8) & 0xFF);
                EEPROM.write(1, (write >> 16) & 0xFF);
                EEPROM.write(0, (write >> 24) & 0xFF);

                sfr::EEPROM::time_of_last_write += time_since_last_write;
            }
        }
    }
}

/* NOTES:
> time_of_last_write holds the time that has passed since the last EEPROM write. This value resets every time the Teensy restarts.
    - As millis() continues to increment for one powered session of the Teensy, the time_of_last_write marks the time of the last
      write for that powered session.
> eeprom_value holds the total accumulated time tracked by EEPROM.
*/

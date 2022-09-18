#include "EEPROMControlTask.hpp"

EEPROMControlTask::EEPROMControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void EEPROMControlTask::execute()
{

    // WAIT TIME TRACKING
    if (!sfr::EEPROM::alloted_time_passed) {
        /* The wait time of two hours was not reached at the previous EEPROM read. Since then, the wait time
        has increased and re-written to EEPROM, and will be read again in the next lines. */
        int byte0 = EEPROM.read(0) << 24;
        int byte1 = EEPROM.read(1) << 16;
        int byte2 = EEPROM.read(2) << 8;
        int byte3 = EEPROM.read(3);
        int eeprom_value = byte0 + byte1 + byte2 + byte3;
#ifdef VERBOSE
        Serial.println("Current EEPROM value: " + String(eeprom_value));
#endif
        if (eeprom_value > sfr::EEPROM::alloted_time) {
            // The newly read eeprom value exceeds the two hour wait time
            sfr::EEPROM::alloted_time_passed = true;
#ifdef VERBOSE
            Serial.println("EEPROM time tracking finished!");
#endif
        } else {
            int time_since_last_write = millis() - sfr::EEPROM::time_of_last_write;

            if (time_since_last_write > sfr::EEPROM::write_step_time) {
                // The last EEPROM write exceeds the interval between writes, so update the EEPROM value
                int write = time_since_last_write + eeprom_value;
#ifdef VERBOSE
                Serial.println("New value to write: " + String(write));
#endif
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
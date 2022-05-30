#include "EEPROMControlTask.hpp"

void EEPROMControlTask::execute()
{
    if (!sfr::EEPROM::alloted_time_passed) {
        sfr::EEPROM::eeprom_value = EEPROM.read(0);

        if (sfr::EEPROM::eeprom_value > sfr::EEPROM::alloted_time) {
            sfr::EEPROM::alloted_time_passed = true;
        } else {
            int time_since_last_write = millis() - sfr::EEPROM::time_of_last_write;

            if (time_since_last_write > sfr::EEPROM::write_step_time) {
                EEPROM.write(0, time_since_last_write + sfr::EEPROM::eeprom_value);
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

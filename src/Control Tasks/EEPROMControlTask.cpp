#include "EEPROMControlTask.hpp"

EEPROMControlTask::EEPROMControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void EEPROMControlTask::execute()
{
    EEPROMControlTask::save_wait_time();
    EEPROMControlTask::save_sfr_data();
}

void EEPROMControlTask::save_wait_time()
{
    // WAIT TIME TRACKING
    if (!sfr::eeprom::alloted_time_passed) {
        /* The wait time of two hours was not reached at the previous EEPROM read. Since then, the wait time
        has increased and re-written to EEPROM, and will be read again in the next lines. */
        int wait_time;
        EEPROM.get(0, wait_time);
#ifdef VERBOSE
        Serial.println("Current wait time value: " + String(wait_time));
#endif
        if (wait_time >= sfr::eeprom::alloted_time) {
            // The newly read eeprom value reaches the two hour wait time
            sfr::eeprom::alloted_time_passed = true;
#ifdef VERBOSE
            Serial.println("EEPROM time tracking finished!");
#endif
        } else {
            int time_since_last_write = millis() - sfr::eeprom::wait_time_last_write_time;

            if (time_since_last_write >= sfr::eeprom::wait_time_write_step_time) {
                // The time since the last EEPROM write reaches the interval between writes, so update the EEPROM value
                int write_value = wait_time + time_since_last_write;
#ifdef VERBOSE
                Serial.println("New value to write: " + String(write_value));
#endif
                EEPROM.put(0, write_value);

                sfr::eeprom::wait_time_last_write_time += time_since_last_write;
            }
        }
    }
}

void EEPROMControlTask::save_sfr_data()
{
    // SAVING SFR DATA
    int time_since_last_write = millis() - sfr::eeprom::sfr_last_write_time;

    if (time_since_last_write >= sfr::eeprom::sfr_write_step_time) {
        // The last EEPROM write reaches the interval between writes, so update the EEPROM values

        if (sfr::eeprom::sfr_address_age == 99000) { // Programmed write limit is less than the actual endurance of 100000 to create a safety buffer
            sfr::eeprom::sfr_address += constants::eeprom::full_offset;
            EEPROM.put(5, sfr::eeprom::sfr_address);
            sfr::eeprom::sfr_address_age = 0;
        }

        if (sfr::eeprom::sfr_address + constants::eeprom::full_offset - 1 >= EEPROM.length()) {
            // The last byte of the current SFR section would exceed EEPROM memory, so there is not enough room for a full SFR store
            sfr::eeprom::storage_full = true;
        } else {
            sfr::eeprom::sfr_address_age++;

            /*
            > Each field is stored in this format: [boolean restore][T value].
            > The restore boolean indicates whether the field should be restored on Teensy boot up.
            > The value is the field's actual value and takes up the memory of type T.
            > If the field should not be restored, then don't bother saving the field value.
            > Once the section's write age exceeds the write endurance, the stores move to the next section.
            */
            for (SFRInterface *s : SFRInterface::sfr_fields_vector) {
                bool restore = s->getRestore();
                int write_address = sfr::eeprom::sfr_address + s->getAddressOffset();
                EEPROM.put(write_address, restore);
                if (restore) {
                    int data_type = s->getDataType();
                    if (data_type == 4)
                        EEPROM.put(write_address + 1, (uint32_t)s->getFieldValue());
                    else if (data_type == 3)
                        EEPROM.put(write_address + 1, (uint16_t)s->getFieldValue());
                    else if (data_type == 2)
                        EEPROM.put(write_address + 1, (uint8_t)s->getFieldValue());
                    else if (data_type == 1)
                        EEPROM.put(write_address + 1, (bool)s->getFieldValue());
                }
                sfr::eeprom::sfr_last_write_time += time_since_last_write;
            }
        }
    }
}

/* NOTES:
> wait_time_last_write_time and sfr_last_write_time hold the millis() count
      of the last write for the current powered session.
> wait_time holds the total accumulated time tracked by EEPROM for the initial 2 hour wait.
> EEPROM write endurance: https://www.pjrc.com/teensy/td_libs_EEPROM.html
*/

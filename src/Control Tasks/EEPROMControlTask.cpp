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
        if (wait_time > sfr::eeprom::alloted_time) {
            // The newly read eeprom value exceeds the two hour wait time
            sfr::eeprom::alloted_time_passed = true;
#ifdef VERBOSE
            Serial.println("EEPROM time tracking finished!");
#endif
        } else {
            int time_since_last_write = millis() - sfr::eeprom::wait_time_last_write_time;

            if (time_since_last_write > sfr::eeprom::wait_time_write_step_time) {
                // The time since the last EEPROM write exceeds the interval between writes, so update the EEPROM value
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

    if (time_since_last_write > sfr::eeprom::sfr_write_step_time) {
        // The last EEPROM write exceeds the interval between writes, so update the EEPROM value
        int sfr_address = sfr::eeprom::sfr_address;

        /*
        > Each field is stored in this format: [boolean restore][T value].
        > The restore boolean indicates whether the field should be restored on Teensy boot up.
        > The value is the field's actual value and takes up the memory of type T.
        > If the field should not be restored, then don't bother saving the field value.
        > Once the section's write age exceeds the write endurance, the stores move to the next section.
        */
        for (SFRInterface *s : SFRInterface::sfr_fields_vector) {
            bool restore = s->getRestore();
            int write_address = sfr_address + s->getAddressOffset();
            EEPROM.put(write_address, restore);
            if (restore) {
                int data_type = s->getDataType();
                if (data_type == 4)
                    EEPROM.put(write_address + 2, (uint32_t)s->getValue());
                else if (data_type == 3)
                    EEPROM.put(write_address + 2, (uint16_t)s->getValue());
                else if (data_type == 2)
                    EEPROM.put(write_address + 2, (uint8_t)s->getValue());
                else if (data_type == 1)
                    EEPROM.put(write_address + 2, (bool)s->getValue());
            }
            sfr::eeprom::sfr_last_write_time += time_since_last_write;
        }

        sfr::eeprom::sfr_address_age++;
        if (sfr::eeprom::sfr_address_age > 100000) {
            sfr::eeprom::sfr_address += constants::eeprom::full_offset;
            EEPROM.put(4, sfr::eeprom::sfr_address);
        }
    }
}

/* NOTES:
> wait_time_last_write_time and sfr_last_write_time hold the millis() count
      of the last write for the current powered session.
> wait_time holds the total accumulated time tracked by EEPROM for the initial 2 hour wait.
> EEPROM write endurance: https://www.pjrc.com/teensy/td_libs_EEPROM.html
*/

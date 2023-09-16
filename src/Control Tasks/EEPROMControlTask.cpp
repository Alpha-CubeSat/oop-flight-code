#include "EEPROMControlTask.hpp"

EEPROMControlTask::EEPROMControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void EEPROMControlTask::execute()
{
    EEPROMControlTask::save_sfr_data();
    EEPROMControlTask::check_wait_time();
}

void EEPROMControlTask::check_wait_time()
{
    // WAIT TIME CHECK
    if (!sfr::eeprom::allotted_time_passed && sfr::mission::mission_time >= constants::timecontrol::allotted_time) {
        // The newly read eeprom value reaches the two hour wait time
        sfr::eeprom::allotted_time_passed = true;
#ifdef VERBOSE
        Serial.println("EEPROM time tracking finished!");
#endif
    }
}

void EEPROMControlTask::save_sfr_data()
{
    // SAVING SFR DATA
    int cycles_since_last_write = sfr::mission::cycle_no - sfr::eeprom::sfr_last_write_cycle;

    if (cycles_since_last_write * constants::timecontrol::control_cycle_time_ms > sfr::eeprom::sfr_write_step_time) {
        // The last EEPROM write exceeds the interval between writes, so update the EEPROM value
        int sfr_address = (int)sfr::eeprom::sfr_address;

        /*
        > Each field is stored in this format: [boolean restore][T value].
        > The restore boolean indicates whether the field should be restored on Teensy boot up.
        > The value is the field's actual value and takes up the memory of type T.
        > If the field should not be restored, then don't bother saving the field value.
        > Once the section's write age exceeds the write endurance, the stores move to the next section.
        */
        for (SFRInterface *s : SFRInterface::sfr_fields_vector) {
            bool restore = s->getRestore();
            /*int write_address = sfr_address + s->getAddressOffset();
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
            }*/
            sfr::eeprom::sfr_last_write_cycle = sfr::mission::cycle_no;
        }

        sfr::eeprom::sfr_address_age++;
        if (sfr::eeprom::sfr_address_age > 99000) { // Programmed write limit is less than the actual endurance of 100000 to create a safety buffer
            // sfr::eeprom::sfr_address += (uint16_t)constants::eeprom::full_offset;
            EEPROM.put(5, sfr::eeprom::sfr_address);
            sfr::eeprom::sfr_address_age = 0;
        }

        // if (sfr::eeprom::sfr_address + constants::eeprom::full_offset - 1 >= EEPROM.length()) {
        if (sfr::eeprom::sfr_address - 1 >= EEPROM.length()) {
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
                /*int write_address = sfr::eeprom::sfr_address + s->getAddressOffset();
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
                }*/
                sfr::eeprom::sfr_last_write_cycle = sfr::mission::cycle_no;
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

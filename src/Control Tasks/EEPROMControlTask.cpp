#include "EEPROMControlTask.hpp"

EEPROMControlTask::EEPROMControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void EEPROMControlTask::execute()
{
    sfr::eeprom::time_alive = millis();

    if (sfr::eeprom::time_alive - sfr::eeprom::last_fast_save_time > constants::eeprom::fast_write_interval && sfr::eeprom::boot_mode) {
        // Adequate time has passed for a new write, and EEPROM is counting time in boot
        EEPROMControlTask::save_boot_time();
        sfr::eeprom::last_fast_save_time = sfr::eeprom::time_alive;
    }

    if (sfr::eeprom::time_alive - sfr::eeprom::last_fast_save_time > constants::eeprom::fast_write_interval && !sfr::eeprom::boot_mode && !sfr::eeprom::error_mode) {
        // Adequate time has passed for a new write, and EEPROM has finished counting time in boot, and EEPROM is valid
        EEPROMControlTask::save_dynamic_data();
        sfr::eeprom::last_fast_save_time = sfr::eeprom::time_alive;
    }

    unsigned int write_interval = constants::eeprom::slow_write_interval;
    if (sfr::eeprom::light_switch) {
        // Save SFR at a faster rate if the light switch is on
        write_interval = constants::eeprom::fast_write_interval;
    }

    if (sfr::eeprom::time_alive - sfr::eeprom::last_sfr_save_time > write_interval && !sfr::eeprom::boot_mode && !sfr::eeprom::error_mode) {
        // Adequate time has passed for a new write, and EEPROM has finished counting time in boot, and EEPROM is valid
        EEPROMControlTask::save_sfr_data();
        sfr::eeprom::last_sfr_save_time = sfr::eeprom::time_alive;
    }
}

void EEPROMControlTask::save_boot_time()
{
    EEPROM.put(constants::eeprom::boot_time_loc1, sfr::eeprom::time_alive.getFieldValue());
    EEPROM.put(constants::eeprom::boot_time_loc2, sfr::eeprom::time_alive.getFieldValue());
}

void EEPROMControlTask::save_dynamic_data()
{
    if (sfr::eeprom::dynamic_data_age == 95000) {
        // If write age reached, shift and save the dynamic data address
        sfr::eeprom::dynamic_data_addr += constants::eeprom::dynamic_data_full_offset;
        EEPROM.put(constants::eeprom::dynamic_data_addr_loc1, sfr::eeprom::dynamic_data_addr.getFieldValue());
        EEPROM.put(constants::eeprom::dynamic_data_addr_loc2, sfr::eeprom::dynamic_data_addr.getFieldValue());
        sfr::eeprom::dynamic_data_age = 0;
    }

    if (sfr::eeprom::dynamic_data_addr + constants::eeprom::dynamic_data_full_offset > constants::eeprom::sfr_data_start) {
        // There is enough memory for another cycle of dynamic data
        sfr::eeprom::dynamic_data_age++;
        EEPROM.put(sfr::eeprom::dynamic_data_addr, sfr::eeprom::time_alive.getFieldValue());
        EEPROM.put(sfr::eeprom::dynamic_data_addr + 4, sfr::eeprom::dynamic_data_age.getFieldValue());
    }
}

void EEPROMControlTask::save_sfr_data()
{
    if (sfr::eeprom::sfr_data_age == 95000) {
        // If write age reached, shift and save the SFR data address
        sfr::eeprom::sfr_data_age += constants::eeprom::sfr_data_full_offset;
        EEPROM.put(constants::eeprom::sfr_data_addr_loc1, sfr::eeprom::sfr_data_addr.getFieldValue());
        EEPROM.put(constants::eeprom::sfr_data_addr_loc2, sfr::eeprom::sfr_data_addr.getFieldValue());
        sfr::eeprom::sfr_data_age = 0;
    }

    if (sfr::eeprom::sfr_data_addr + constants::eeprom::sfr_data_full_offset > constants::eeprom::boot_counter_loc2) {
        // There is enough memory for another cycle of SFR data

        // Update and write SFR data age
        sfr::eeprom::sfr_data_age++;
        EEPROM.put(sfr::eeprom::sfr_data_addr, sfr::eeprom::sfr_data_age.getFieldValue());

        // Save SFR fields
        uint16_t sfr_write_address = sfr::eeprom::sfr_data_addr + 4;
        for (const auto &pair : SFRInterface::opcode_lookup) {
            EEPROM.put(sfr_write_address, pair.second->getRestoreOnBoot());
            EEPROM.put(sfr_write_address + 1, pair.second->getFieldValue());
            sfr_write_address += constants::eeprom::sfr_store_size;
        }

        // Write SFR data age again. EEPROM restore will check that these ages match to determine if the last SFR save was completed.
        EEPROM.put(sfr_write_address, sfr::eeprom::sfr_data_age.getFieldValue());
    }
}

/* NOTES:
 * EEPROM write endurance: https://www.pjrc.com/teensy/td_libs_EEPROM.html
 */

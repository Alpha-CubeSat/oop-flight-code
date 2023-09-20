#include "EEPROMControlTask.hpp"

EEPROMControlTask::EEPROMControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void EEPROMControlTask::execute()
{
    sfr::eeprom::time_alive = millis();

    if (sfr::eeprom::time_alive - sfr::eeprom::last_fast_save_time > constants::eeprom::fast_write_interval && sfr::eeprom::boot_mode) {
        EEPROMControlTask::save_boot_time();
        sfr::eeprom::last_fast_save_time = sfr::eeprom::time_alive;
    }

    if (sfr::eeprom::time_alive - sfr::eeprom::last_fast_save_time > constants::eeprom::fast_write_interval && !sfr::eeprom::boot_mode && !sfr::eeprom::error_mode) {
        EEPROMControlTask::save_dynamic_data();
        sfr::eeprom::last_fast_save_time = sfr::eeprom::time_alive;
    }

    unsigned int write_interval = constants::eeprom::slow_write_interval;
    if (sfr::eeprom::light_switch) {
        write_interval = constants::eeprom::fast_write_interval;
    }

    if (sfr::eeprom::time_alive - sfr::eeprom::last_sfr_save_time > write_interval && !sfr::eeprom::boot_mode && !sfr::eeprom::error_mode) {
        EEPROMControlTask::save_sfr_data();
        sfr::eeprom::last_sfr_save_time = sfr::eeprom::time_alive;
    }
}

void EEPROMControlTask::save_boot_time()
{
    EEPROM.put(constants::eeprom::boot_time_loc1, sfr::eeprom::time_alive);
    EEPROM.put(constants::eeprom::boot_time_loc2, sfr::eeprom::time_alive);
}

void EEPROMControlTask::save_dynamic_data()
{
    if (sfr::eeprom::dynamic_data_age == 95000) {
        sfr::eeprom::dynamic_data_addr += constants::eeprom::dynamic_data_full_offset;
        EEPROM.put(constants::eeprom::dynamic_data_addr_loc1, sfr::eeprom::dynamic_data_addr);
        EEPROM.put(constants::eeprom::dynamic_data_addr_loc2, sfr::eeprom::dynamic_data_addr);
        sfr::eeprom::dynamic_data_age = 0;
    }

    if (sfr::eeprom::dynamic_data_addr + constants::eeprom::dynamic_data_full_offset > constants::eeprom::sfr_data_start) {

        sfr::eeprom::dynamic_data_age++;
        EEPROM.put(sfr::eeprom::dynamic_data_addr, sfr::eeprom::time_alive);
        EEPROM.put(sfr::eeprom::dynamic_data_addr + 4, sfr::eeprom::dynamic_data_age);
    }
}

void EEPROMControlTask::save_sfr_data()
{
    if (sfr::eeprom::sfr_data_age == 95000) {
        sfr::eeprom::sfr_data_age += constants::eeprom::sfr_data_full_offset;
        EEPROM.put(constants::eeprom::sfr_data_addr_loc1, sfr::eeprom::sfr_data_addr);
        EEPROM.put(constants::eeprom::sfr_data_addr_loc2, sfr::eeprom::sfr_data_addr);
        sfr::eeprom::sfr_data_age = 0;
    }

    if (sfr::eeprom::sfr_data_addr + constants::eeprom::sfr_data_full_offset > constants::eeprom::boot_counter_loc2) {
        sfr::eeprom::sfr_data_age++;
        EEPROM.put(sfr::eeprom::sfr_data_addr, sfr::eeprom::sfr_data_age);
        uint16_t sfr_write_address = sfr::eeprom::sfr_data_addr + 4;
        for (const auto &pair : SFRInterface::opcode_lookup) {
            EEPROM.put(sfr_write_address, pair.second->getRestoreOnBoot());
            EEPROM.put(sfr_write_address + 1, pair.second->getFieldValue());
            sfr_write_address += constants::eeprom::sfr_store_size;
        }

        EEPROM.put(sfr_write_address, sfr::eeprom::sfr_data_age);
    }
}

/* NOTES:
 * EEPROM write endurance: https://www.pjrc.com/teensy/td_libs_EEPROM.html
 */

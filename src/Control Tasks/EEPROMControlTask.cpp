#include "EEPROMControlTask.hpp"

EEPROMControlTask::EEPROMControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    fast_cycle_counter = 0;
    slow_cycle_counter = 0;
    prev_time = 0;
}

void EEPROMControlTask::execute()
{
    uint32_t curr_time = millis();
    uint32_t new_time_increment = curr_time - prev_time;
    sfr::eeprom::time_alive += new_time_increment;
    prev_time = curr_time;

    if (sfr::eeprom::light_switch != last_light_switch) {
        EEPROM.put(constants::eeprom::light_switch_loc1, sfr::eeprom::light_switch.get());
        EEPROM.put(constants::eeprom::light_switch_loc2, sfr::eeprom::light_switch.get());
    }

    if (fast_cycle_counter == 0 && sfr::eeprom::boot_mode) {
        // Adequate time has passed for a new write, and EEPROM is counting time in boot
        EEPROMControlTask::save_boot_time();
    }

    if (fast_cycle_counter == 0 && !sfr::eeprom::boot_mode && !sfr::eeprom::error_mode) {
        // Adequate time has passed for a new write, and EEPROM has finished counting time in boot, and EEPROM is valid
        EEPROMControlTask::save_dynamic_data();
    }

    uint8_t sfr_cycle_counter = slow_cycle_counter;
    if (sfr::eeprom::light_switch) {
        // Save SFR at a faster rate if the light switch is on
        sfr_cycle_counter = fast_cycle_counter;
    }

    if (sfr_cycle_counter == 0 && !sfr::eeprom::boot_mode && !sfr::eeprom::error_mode) {
        // Adequate time has passed for a new write, and EEPROM has finished counting time in boot, and EEPROM is valid
        EEPROMControlTask::save_sfr_data();
    }

    fast_cycle_counter++;
    if (fast_cycle_counter == constants::eeprom::fast_write_interval) {
        fast_cycle_counter = 0;
    }

    slow_cycle_counter++;
    if (slow_cycle_counter == constants::eeprom::slow_write_interval) {
        slow_cycle_counter = 0;
    }
}

void EEPROMControlTask::save_boot_time()
{
    EEPROM.put(constants::eeprom::boot_time_loc1, sfr::eeprom::time_alive.get());
    EEPROM.put(constants::eeprom::boot_time_loc2, sfr::eeprom::time_alive.get());

    if (sfr::eeprom::time_alive >= 2 * constants::time::one_hour) {
        sfr::eeprom::boot_mode = false;
    }
}

void EEPROMControlTask::save_dynamic_data()
{
    if (sfr::eeprom::dynamic_data_age == constants::eeprom::write_age_limit) {
        // If write age reached, shift and save the dynamic data address
        sfr::eeprom::dynamic_data_addr += constants::eeprom::dynamic_data_full_offset;
        EEPROM.put(constants::eeprom::dynamic_data_addr_loc1, sfr::eeprom::dynamic_data_addr.get());
        EEPROM.put(constants::eeprom::dynamic_data_addr_loc2, sfr::eeprom::dynamic_data_addr.get());
        sfr::eeprom::dynamic_data_age = 0;
    }

    if (sfr::eeprom::dynamic_data_addr + constants::eeprom::dynamic_data_full_offset <= sfr::eeprom::dynamic_data_addr.getMax()) {
        // There is enough memory for another cycle of dynamic data
        sfr::eeprom::dynamic_data_age++;
        EEPROM.put(sfr::eeprom::dynamic_data_addr, sfr::eeprom::time_alive.get());
        EEPROM.put(sfr::eeprom::dynamic_data_addr + 4, sfr::eeprom::dynamic_data_age.get());
    }
}

void EEPROMControlTask::save_sfr_data()
{
    if (sfr::eeprom::sfr_data_age == constants::eeprom::write_age_limit) {
        // If write age reached, shift and save the SFR data address
        sfr::eeprom::sfr_data_age += constants::eeprom::sfr_data_full_offset;
        EEPROM.put(constants::eeprom::sfr_data_addr_loc1, sfr::eeprom::sfr_data_addr.get());
        EEPROM.put(constants::eeprom::sfr_data_addr_loc2, sfr::eeprom::sfr_data_addr.get());
        sfr::eeprom::sfr_data_age = 0;
    }

    if (sfr::eeprom::sfr_data_addr + constants::eeprom::sfr_data_full_offset <= sfr::eeprom::sfr_data_addr.getMax()) {
        // There is enough memory for another cycle of SFR data

        // Update and write SFR data age
        sfr::eeprom::sfr_data_age++;
        EEPROM.put(sfr::eeprom::sfr_data_addr, sfr::eeprom::sfr_data_age.get());

        // Save each SFR field's restore boolean and value as a uint32_t
        uint16_t sfr_write_address = sfr::eeprom::sfr_data_addr + 4;
        for (const auto &pair : SFRInterface::opcode_lookup) {
            EEPROM.put(sfr_write_address, pair.second->getRestoreOnBoot());
            EEPROM.put(sfr_write_address + 1, pair.second->getFieldValue());
            sfr_write_address += constants::eeprom::sfr_store_size;
        }

        // Write SFR data age again. EEPROM restore will check that these ages match to determine if the last SFR save was completed.
        EEPROM.put(sfr_write_address, sfr::eeprom::sfr_data_age.get());
    }
}

/* NOTES:
 * EEPROM write endurance: https://www.pjrc.com/teensy/td_libs_EEPROM.html
 */

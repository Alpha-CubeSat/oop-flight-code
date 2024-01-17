#include "EEPROMControlTask.hpp"

EEPROMControlTask::EEPROMControlTask()
{
    fast_cycle_counter = 0;
    slow_cycle_counter = 0;
    prev_time = 0;
}

void EEPROMControlTask::execute()
{
    sfr::eeprom::time_alive += millis() - prev_time;
    prev_time = millis();

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

    uint32_t sfr_cycle_counter = slow_cycle_counter;
    if (sfr::eeprom::light_switch) {
        // Save SFR at a faster rate if the light switch is on
        sfr_cycle_counter = fast_cycle_counter;
    }

    if (sfr_cycle_counter == 0 && !sfr::eeprom::boot_mode && !sfr::eeprom::error_mode) {
        // Adequate time has passed for a new write, and EEPROM has finished counting time in boot, and EEPROM is valid
        EEPROMControlTask::save_sfr_data();
    }

    if (fast_cycle_counter == constants::eeprom::fast_write_interval - 1) {
        fast_cycle_counter = 0;
    } else {
        fast_cycle_counter++;
    }

    if (slow_cycle_counter == constants::eeprom::slow_write_interval - 1) {
        slow_cycle_counter = 0;
    } else {
        slow_cycle_counter++;
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

    if (sfr::eeprom::dynamic_data_addr + constants::eeprom::dynamic_data_full_offset - 1 <= sfr::eeprom::dynamic_data_addr.getMax()) {
        // There is enough memory for another cycle of dynamic data

        // Update dynamic data age
        sfr::eeprom::dynamic_data_age++;

        // Save dynamic data to EEPROM
        EEPROM.put(sfr::eeprom::dynamic_data_addr, sfr::eeprom::time_alive.get());
        EEPROM.put(sfr::eeprom::dynamic_data_addr + 4, sfr::eeprom::dynamic_data_age.get());
    }
}

void EEPROMControlTask::save_sfr_data()
{
    if (sfr::eeprom::sfr_data_age == constants::eeprom::write_age_limit) {
        // If write age reached, shift and save the SFR data address
        
        sfr::eeprom::sfr_data_addr += constants::eeprom::sfr_data_full_offset;
        EEPROM.put(constants::eeprom::sfr_data_addr_loc1, sfr::eeprom::sfr_data_addr.get());
        EEPROM.put(constants::eeprom::sfr_data_addr_loc2, sfr::eeprom::sfr_data_addr.get());
        sfr::eeprom::sfr_data_age = 0;
    }

    if (sfr::eeprom::sfr_data_addr + constants::eeprom::sfr_data_full_offset - 1 <= sfr::eeprom::sfr_data_addr.getMax()) {
        // There is enough memory for another cycle of SFR data

        // Update SFR data age
        sfr::eeprom::sfr_data_age++;

        // Save each SFR field's restore boolean and value as a uint32_t
        uint16_t sfr_write_address = sfr::eeprom::sfr_data_addr + 4;
        for (const auto &pair : SFRInterface::opcode_lookup) {
            EEPROM.put(sfr_write_address, pair.second->getRestoreOnBoot());
            EEPROM.put(sfr_write_address + 1, pair.second->getFieldValue());
            sfr_write_address += constants::eeprom::sfr_store_size;
        }

        uint32_t checksum = generate_sfr_checksum();

        // Write SFR checksum. EEPROM restore will check that its own generated checksum matches to determine if the last SFR save was completed.
        EEPROM.put(sfr::eeprom::sfr_data_addr, checksum);
    }
}

uint32_t EEPROMControlTask::generate_sfr_checksum()
{
    // SFR checksum accumulator
    uint32_t checksum_total = 0;
    uint16_t sfr_start_addr = sfr::eeprom::sfr_data_addr + 4;
    uint16_t sfr_end_addr = sfr::eeprom::sfr_data_addr + constants::eeprom::sfr_data_full_offset;

    for (unsigned int i = sfr_start_addr; i < sfr_end_addr; i++) {
        uint8_t value = EEPROM.read(i);
        checksum_total += value;
    }

    uint32_t checksum = checksum_total % constants::eeprom::sfr_num_fields;
    return checksum;
}

/* NOTES:
 * EEPROM write endurance: https://www.pjrc.com/teensy/td_libs_EEPROM.html
 */

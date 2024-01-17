#include "EEPROMRestore.hpp"

void EEPROMRestore::execute()
{
    // Determines if satellite is still in boot mode or if boot values indicate an EEPROM error
    check_boot_vals();

    // If boot checks did not indicate EEPROM error, check if blue moon data indicates an EEPROM error
    // If not, restore blue moon data
    if (!sfr::eeprom::error_mode) {
        restore_blue_moon_data();
    }

    // If EEPROM data is still valid after blue moon data checks and boot wait is over,
    // restore dynamic data and SFR data
    if (!sfr::eeprom::error_mode && !sfr::eeprom::boot_mode) {
        restore_dynamic_data();
        restore_sfr_data();
    }
}

void EEPROMRestore::check_boot_vals()
{
    uint32_t boot_time1;
    EEPROM.get(constants::eeprom::boot_time_loc1, boot_time1);
    uint32_t boot_time2;
    EEPROM.get(constants::eeprom::boot_time_loc2, boot_time2);

    uint8_t boot_counter1;
    EEPROM.get(constants::eeprom::boot_counter_loc1, boot_counter1);
    uint8_t boot_counter2;
    EEPROM.get(constants::eeprom::boot_counter_loc2, boot_counter2);

    if (boot_time1 == boot_time2 && boot_counter1 == boot_counter2) {
        // Boot time counters and boot counters are valid
        
        sfr::eeprom::error_mode = false;

        sfr::eeprom::boot_counter = boot_counter1 + 1;
        EEPROM.put(constants::eeprom::boot_counter_loc1, sfr::eeprom::boot_counter.get());
        EEPROM.put(constants::eeprom::boot_counter_loc2, sfr::eeprom::boot_counter.get());
        if (boot_time1 < 2 * constants::time::one_hour) {
            // The initial two hour wait has not passed, still in boot mode
            
            sfr::eeprom::boot_mode = true;
            sfr::eeprom::time_alive = boot_time1;
        } else {
            sfr::eeprom::boot_mode = false;
            // Do not set time alive, boot time only counts up to two hours
            // It will be restored with the dynamic data now that the boot phase is over
        }
    } else {
        // Boot time counters or boot counters are invalid, restart initial two hour wait
        
        sfr::eeprom::error_mode = true;
        sfr::eeprom::boot_mode = true;
        sfr::eeprom::boot_restarted = true;

        sfr::eeprom::time_alive = 0;
        EEPROM.put(constants::eeprom::boot_time_loc1, sfr::eeprom::time_alive.get());
        EEPROM.put(constants::eeprom::boot_time_loc2, sfr::eeprom::time_alive.get());

        sfr::eeprom::boot_counter = 1;
        EEPROM.put(constants::eeprom::boot_counter_loc1, sfr::eeprom::boot_counter.get());
        EEPROM.put(constants::eeprom::boot_counter_loc2, sfr::eeprom::boot_counter.get());
    }
}

void EEPROMRestore::restore_blue_moon_data()
{
    // Fetch the blue moon data for restoring dynamic and SFR data
    bool light_switch1;
    EEPROM.get(constants::eeprom::light_switch_loc1, light_switch1);
    bool light_switch2;
    EEPROM.get(constants::eeprom::light_switch_loc2, light_switch2);

    uint16_t dynamic_data_addr1;
    EEPROM.get(constants::eeprom::dynamic_data_addr_loc1, dynamic_data_addr1);
    uint16_t dynamic_data_addr2;
    EEPROM.get(constants::eeprom::dynamic_data_addr_loc2, dynamic_data_addr2);

    uint16_t sfr_data_addr1;
    EEPROM.get(constants::eeprom::sfr_data_addr_loc1, sfr_data_addr1);
    uint16_t sfr_data_addr2;
    EEPROM.get(constants::eeprom::sfr_data_addr_loc2, sfr_data_addr2);

    if (light_switch1 == light_switch2 && dynamic_data_addr1 == dynamic_data_addr2 && sfr_data_addr1 == sfr_data_addr2) {
        // EEPROM data is still valid

        sfr::eeprom::dynamic_data_addr = dynamic_data_addr1;
        sfr::eeprom::sfr_data_addr = sfr_data_addr1;
        sfr::eeprom::light_switch = light_switch1;
    } else {
        // EEPROM data is invalid

        sfr::eeprom::error_mode = true;
    }
}

void EEPROMRestore::restore_dynamic_data()
{
    bool has_dynamic_data_space = sfr::eeprom::dynamic_data_addr + constants::eeprom::dynamic_data_full_offset - 1 <= sfr::eeprom::dynamic_data_addr.getMax();

    if (has_dynamic_data_space) {
        // EEPROM for dynamic data is not full, so restore dynamic data
        
        uint32_t time_alive;
        EEPROM.get(sfr::eeprom::dynamic_data_addr, time_alive);
        sfr::eeprom::time_alive = time_alive;

        uint32_t dynamic_data_age;
        EEPROM.get(sfr::eeprom::dynamic_data_addr + 4, dynamic_data_age);
        sfr::eeprom::dynamic_data_age = dynamic_data_age;
    }
}

void EEPROMRestore::restore_sfr_data()
{
    bool has_sfr_data_space = sfr::eeprom::sfr_data_addr + constants::eeprom::sfr_data_full_offset - 1 <= sfr::eeprom::sfr_data_addr.getMax();
    if (has_sfr_data_space) {
        // EEPROM for SFR data is not full, so proceed with further checks to restore SFR data

        // Check SFR data integrity with checksum
        uint32_t stored_checksum;
        EEPROM.get(sfr::eeprom::sfr_data_addr, stored_checksum);
        uint32_t generated_checksum = EEPROMControlTask::generate_sfr_checksum();
        sfr::eeprom::sfr_save_completed = stored_checksum == generated_checksum; // False means SFR didn't finish saving

        if (sfr::eeprom::sfr_save_completed) {
            
            // Pull the SFR data write age from the back of the current EEPROM section
            uint32_t sfr_data_age;
            uint16_t sfr_data_age_addr = sfr::eeprom::sfr_data_addr + constants::eeprom::sfr_data_full_offset - 4;
            EEPROM.get(sfr_data_age_addr, sfr_data_age);
            sfr::eeprom::sfr_data_age = sfr_data_age;
            
            if (sfr::eeprom::light_switch) {

                // Move the read address back to the EEPROM location of the first SFR field
                uint16_t sfr_read_address = sfr::eeprom::sfr_data_addr + 4;

                // Restore SFR fields according to their restore booleans
                for (const auto &pair : SFRInterface::opcode_lookup) {
                    bool restore;
                    EEPROM.get(sfr_read_address, restore);
                    pair.second->setRestoreOnBoot(restore);

                    if (restore) {
                        uint32_t value;
                        EEPROM.get(sfr_read_address + 1, value);
                        pair.second->setFieldValue(value);
                    }

                    sfr_read_address += constants::eeprom::sfr_store_size;
                }
            }
        }
    }
}
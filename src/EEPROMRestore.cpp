#include "EEPROMRestore.hpp"

void EEPROMRestore::execute()
{
    // Determines if satellite is still in boot mode
    restore_and_check_boot_vals();

    // If boot checks did not indicate EEPROM error,
    // restore blue moon data and check if they indicate an EEPROM error
    try_restore_and_check_blue_moon_data();

    // If EEPROM data is still valid after blue moon data checks and boot wait is over,
    // restore dynamic data and try restoring SFR data
    try_restore_dynamic_and_sfr_data();

    Serial.println(sfr::eeprom::boot_counter);
}

void EEPROMRestore::restore_and_check_boot_vals()
{
    Serial.println("WoRK BITCH");
    uint32_t boot_time1;
    EEPROM.get(constants::eeprom::boot_time_loc1, boot_time1);
    uint32_t boot_time2;
    EEPROM.get(constants::eeprom::boot_time_loc2, boot_time2);

    uint8_t boot_counter1;
    EEPROM.get(constants::eeprom::boot_counter_loc1, boot_counter1);
    uint8_t boot_counter2;
    EEPROM.get(constants::eeprom::boot_counter_loc2, boot_counter2);

    Serial.println("in restore");
    Serial.println(constants::eeprom::boot_time_loc1);
    Serial.println(constants::eeprom::boot_time_loc2);
    Serial.println(constants::eeprom::boot_counter_loc1);
    Serial.println(constants::eeprom::boot_counter_loc2);

    Serial.println(boot_time1);
    Serial.println(boot_time2);
    Serial.println(boot_counter1);
    Serial.println(boot_counter2);
    Serial.println("-----");

    if (boot_time1 == boot_time2 && boot_counter1 == boot_counter2) {
        // Boot time counters and boot counters are valid
        sfr::eeprom::error_mode = false;

        sfr::eeprom::boot_counter = boot_counter1 + 1;
        Serial.println(sfr::eeprom::boot_counter);
        EEPROM.put(constants::eeprom::boot_counter_loc1, sfr::eeprom::boot_counter.get());
        EEPROM.put(constants::eeprom::boot_counter_loc2, sfr::eeprom::boot_counter.get());
        if (boot_time1 < 2 * constants::time::one_hour) {
            // The initial two hour wait has not passed, still in boot mode
            sfr::eeprom::boot_mode = true;
            sfr::eeprom::time_alive = boot_time1;
        } else {
            sfr::eeprom::boot_mode = false;
            // Do not set time alive, boot time only counts up to two hours
            // It will be restored with the dynamic data dnow that the boot phase is over
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

void EEPROMRestore::try_restore_and_check_blue_moon_data()
{
    if (!sfr::eeprom::error_mode) {
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
}

void EEPROMRestore::try_restore_dynamic_and_sfr_data()
{
    if (!sfr::eeprom::error_mode) {

        bool dynamic_data_space_filled = false;
        bool sfr_data_space_filled = false;
        if (sfr::eeprom::dynamic_data_addr + constants::eeprom::dynamic_data_full_offset > sfr::eeprom::dynamic_data_addr.getMax()) {
            // EEPROM has been filled up and no more dynamic data saves have been done, so do not attempt to restore
            dynamic_data_space_filled = true;
        }
        if (sfr::eeprom::sfr_data_addr + constants::eeprom::sfr_data_full_offset > sfr::eeprom::sfr_data_addr.getMax()) {
            // EEPROM has been filled up and no more SFR data saves have been done, so do not attempt to restore
            sfr_data_space_filled = true;
        }
        
        if (!sfr::eeprom::boot_mode && !dynamic_data_space_filled)
        {
            // EEPROM for dynamic data is not full, so fetch dynamic metadata
            uint32_t time_alive;
            EEPROM.get(sfr::eeprom::dynamic_data_addr, time_alive);
            sfr::eeprom::time_alive = time_alive;

            uint32_t dynamic_data_age;
            EEPROM.get(sfr::eeprom::dynamic_data_addr + 4, dynamic_data_age);
            sfr::eeprom::dynamic_data_age = dynamic_data_age;
        }

        if (sfr::eeprom::light_switch && !sfr_data_space_filled) {
            // Light switch is on, EEPROM for SFR is not full
            // Try to restore SFR fields according to their restore booleans

            uint32_t sfr_data_age1;
            EEPROM.get(sfr::eeprom::sfr_data_addr, sfr_data_age1);

            uint32_t sfr_data_age2;
            EEPROM.get(sfr::eeprom::sfr_data_addr + constants::eeprom::sfr_data_full_offset - 4, sfr_data_age2);

            sfr::eeprom::sfr_save_completed = sfr_data_age1 == sfr_data_age2; // False means SFR didn't finish saving
            sfr::eeprom::sfr_data_age = sfr_data_age1;                        // First SFR age is more accurate

            if (sfr::eeprom::sfr_save_completed) {
                uint16_t sfr_read_address = sfr::eeprom::sfr_data_addr + 4;

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
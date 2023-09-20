#include "EEPROMRestore.hpp"
// #include <algorithm> // find

void EEPROMRestore::execute()
{
    uint32_t boot_time1;
    EEPROM.get(constants::eeprom::boot_time_loc1, boot_time1);
    uint32_t boot_time2;
    EEPROM.get(constants::eeprom::boot_time_loc2, boot_time2);

    if (boot_time1 == boot_time2) {
        // Boot time counters are valid, check if initial two hour wait has passed
        if (boot_time1 < 2 * constants::time::one_hour) {
            sfr::eeprom::boot_mode = true;
            sfr::eeprom::time_alive = boot_time1;
        } else {
            sfr::eeprom::boot_mode = false;
            // Do not set time alive, boot time only counts up to two hours
            // It will be restored with the dynamic data updated in the EEPROM Control Task now that the boot phase is over
        }
    } else {
        // Boot time counters are invalid, zero out counters and start initial two hour wait
        EEPROM.put(constants::eeprom::boot_time_loc1, (uint32_t)0);
        EEPROM.put(constants::eeprom::boot_time_loc2, (uint32_t)0);
        sfr::eeprom::boot_mode = true;
        sfr::eeprom::time_alive = 0;
        sfr::eeprom::error_mode = true;
    }

    if (!sfr::eeprom::boot_mode && !sfr::eeprom::error_mode) {
        // Not in boot mode, and boot time check didn't indicate EEPROM error

        // Fetch blue moon data
        uint16_t boot_counter1;
        EEPROM.get(constants::eeprom::boot_counter_loc1, boot_counter1);
        uint16_t boot_counter2;
        EEPROM.get(constants::eeprom::boot_counter_loc2, boot_counter2);

        uint16_t dynamic_data_addr1;
        EEPROM.get(constants::eeprom::dynamic_data_addr_loc1, dynamic_data_addr1);
        uint16_t dynamic_data_addr2;
        EEPROM.get(constants::eeprom::dynamic_data_addr_loc2, dynamic_data_addr2);

        uint16_t sfr_data_addr1;
        EEPROM.get(constants::eeprom::sfr_data_addr_loc1, sfr_data_addr1);
        uint16_t sfr_data_addr2;
        EEPROM.get(constants::eeprom::sfr_data_addr_loc2, sfr_data_addr2);

        bool light_switch1;
        EEPROM.get(constants::eeprom::light_switch_loc1, light_switch1);
        bool light_switch2;
        EEPROM.get(constants::eeprom::light_switch_loc2, light_switch2);

        if (boot_counter1 == boot_counter2 && dynamic_data_addr1 == dynamic_data_addr2 && sfr_data_addr1 == sfr_data_addr2 && light_switch1 == light_switch2) {
            // EEPROM data is valid
            sfr::eeprom::error_mode = false;
            sfr::eeprom::boot_counter = boot_counter1 + 1;
            EEPROM.get(constants::eeprom::boot_counter_loc1, sfr::eeprom::boot_counter);
            EEPROM.get(constants::eeprom::boot_counter_loc2, sfr::eeprom::boot_counter);
            sfr::eeprom::dynamic_data_addr = dynamic_data_addr1;
            sfr::eeprom::sfr_data_addr = sfr_data_addr1;
            sfr::eeprom::light_switch = light_switch1;
        } else {
            // EEPROM data is invalid
            sfr::eeprom::error_mode = true;
        }

        if (!sfr::eeprom::error_mode && sfr::eeprom::light_switch) {
            // EEPROM data is still valid after checks and light switch is on

            // Fetch dynamic metadata
            uint32_t time_alive;
            EEPROM.get(sfr::eeprom::dynamic_data_addr, time_alive);
            sfr::eeprom::time_alive = time_alive;

            uint32_t dynamic_data_age;
            EEPROM.get(sfr::eeprom::dynamic_data_addr + 4, dynamic_data_age);
            sfr::eeprom::dynamic_data_age = dynamic_data_age;

            uint32_t sfr_data_age1;
            EEPROM.get(sfr::eeprom::sfr_data_addr, sfr_data_age1);

            uint32_t sfr_data_age2;
            EEPROM.get(sfr::eeprom::sfr_data_addr + constants::eeprom::sfr_data_full_offset - 4, sfr_data_age2);

            sfr::eeprom::sfr_save_completed = sfr_data_age1 == sfr_data_age2; // Inequality means SFR didn't finish saving
            sfr::eeprom::sfr_data_age = sfr_data_age1;                        // First age is the more accurate count

            // If last SFR save was completed, restore desired SFR fields
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
#ifndef _EEPROMRESETCOMMAND_HPP_
#define _EEPROMRESETCOMMAND_HPP_

#include "SFRField.hpp"
#include "constants.hpp"
#include "sfr.hpp"
#include <EEPROM.h>

class EEPROMResetCommand : public RockblockCommand
{
public:
    EEPROMResetCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};

    void execute()
    {
        // Unpack args
        uint8_t boot_count = (constants::masks::uint32_byte1_mask & f_arg_1) >> 24;
        bool light_switch = !!((constants::masks::uint32_byte2_mask & f_arg_1) >> 16);

        uint16_t sfr_data_addr = (constants::masks::uint32_byte3_mask | constants::masks::uint32_byte4_mask) & f_arg_1;

        uint8_t sfr_data_age = (constants::masks::uint32_byte1_mask & f_arg_2) >> 24;
        uint8_t dynamic_data_age = (constants::masks::uint32_byte2_mask & f_arg_2) >> 16;

        uint16_t dynamic_data_addr = (constants::masks::uint32_byte3_mask | constants::masks::uint32_byte4_mask) & f_arg_2;

        sfr::eeprom::boot_counter = boot_count;
        sfr::eeprom::light_switch = light_switch;
        sfr::eeprom::dynamic_data_addr = dynamic_data_addr;
        sfr::eeprom::sfr_data_addr = sfr_data_addr;
        sfr::eeprom::dynamic_data_age = round(map(dynamic_data_age, 0, 255, 0, constants::eeprom::write_age_limit));
        sfr::eeprom::sfr_data_age = round(map(sfr_data_age, 0, 255, 0, constants::eeprom::write_age_limit));
        sfr::eeprom::error_mode = false;
        sfr::eeprom::boot_restarted = false;

        // Write to EEPROM
        EEPROM.put(constants::eeprom::boot_time_loc1, sfr::eeprom::boot_counter);
        EEPROM.put(constants::eeprom::boot_time_loc2, sfr::eeprom::boot_counter);

        EEPROM.put(constants::eeprom::light_switch_loc1, sfr::eeprom::light_switch);
        EEPROM.put(constants::eeprom::light_switch_loc2, sfr::eeprom::light_switch);

        EEPROM.put(constants::eeprom::dynamic_data_addr_loc1, sfr::eeprom::dynamic_data_addr);
        EEPROM.put(constants::eeprom::dynamic_data_addr_loc2, sfr::eeprom::dynamic_data_addr);

        EEPROM.put(constants::eeprom::sfr_data_addr_loc1, sfr::eeprom::sfr_data_addr);
        EEPROM.put(constants::eeprom::sfr_data_addr_loc2, sfr::eeprom::sfr_data_addr);

        // Write to the relevant SFRFields
    }

    bool isValid()
    {
        return sfr::eeprom::error_mode.get();
    }
};

#endif
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

        uint8_t sfr_data_addr_1 = (constants::masks::uint32_byte3_mask & f_arg_1) >> 8;
        uint8_t sfr_data_addr_2 = (constants::masks::uint32_byte4_mask & f_arg_1);
        uint16_t sfr_data_addr = sfr_data_addr_1 | sfr_data_addr_2;

        uint8_t sfr_data_age = (constants::masks::uint32_byte1_mask & f_arg_2) >> 24;
        uint8_t dynamic_data_age = (constants::masks::uint32_byte2_mask & f_arg_2) >> 16;

        uint8_t dynamic_data_addr_1 = (constants::masks::uint32_byte3_mask & f_arg_2) >> 8;
        uint8_t dynamic_data_addr_2 = (constants::masks::uint32_byte4_mask & f_arg_2);
        uint16_t dynamic_data_addr = dynamic_data_addr_1 | dynamic_data_addr_2;

        // Write to EEPROM
        EEPROM.put(constants::eeprom::boot_time_loc1, (uint8_t)boot_count);
        EEPROM.put(constants::eeprom::boot_time_loc2, (uint8_t)boot_count);

        EEPROM.put(constants::eeprom::light_switch_loc1, light_switch);
        EEPROM.put(constants::eeprom::light_switch_loc2, light_switch);

        EEPROM.put(constants::eeprom::dynamic_data_addr_loc1, (uint16_t)dynamic_data_addr);
        EEPROM.put(constants::eeprom::dynamic_data_addr_loc2, (uint16_t)dynamic_data_addr);

        EEPROM.put(constants::eeprom::sfr_data_addr_loc1, (uint16_t)sfr_data_addr);
        EEPROM.put(constants::eeprom::sfr_data_addr_loc2, (uint16_t)sfr_data_addr);

        // Write to the relevant SFRFields
        SFRInterface::setFieldValByOpcode(0x2800, boot_count);
        SFRInterface::setFieldValByOpcode(0x2802, light_switch);

        SFRInterface::setFieldValByOpcode(0x2805, dynamic_data_addr);
        SFRInterface::setFieldValByOpcode(0x2806, sfr_data_addr);

        SFRInterface::setFieldValByOpcode(0x2808, round(map(dynamic_data_age, 0, 255, 0, 95000)));
        SFRInterface::setFieldValByOpcode(0x2809, round(map(sfr_data_age, 0, 255, 0, 95000)));

        // Set the error mode to false
        SFRInterface::setFieldValByOpcode(0x2801, false);
    }

    bool isValid()
    {
        return sfr::eeprom::error_mode.get();
    }
};

#endif
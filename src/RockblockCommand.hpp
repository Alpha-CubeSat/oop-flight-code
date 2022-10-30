#ifndef _ROCKBLOCKCOMMAND_HPP_
#define _ROCKBLOCKCOMMAND_HPP_

#include "Arduino.h"
#include "SFRField.hpp"
#include "constants.hpp"
#include <stdint.h>

class RawRockblockCommand
{
public:
    uint8_t opcode[2];
    uint8_t arg_1[4];
    uint8_t arg_2[4];

    uint16_t get_f_opcode()
    {
        return (this->opcode[0] << 8) | (this->opcode[1]);
    }

    uint32_t get_f_arg_1()
    {
        return (this->arg_1[0] << 24) | (this->arg_1[1] << 16) | (this->arg_1[2] << 8) | (this->arg_1[3]);
    }

    uint32_t get_f_arg_2()
    {
        return (this->arg_2[0] << 24) | (this->arg_2[1] << 16) | (this->arg_2[2] << 8) | (this->arg_2[3]);
    }
};

class RockblockCommand
{
public:
    uint16_t f_opcode;
    uint32_t f_arg_1;
    uint32_t f_arg_2;

    RockblockCommand(RawRockblockCommand raw)
    {
        this->f_opcode = raw.get_f_opcode();
        this->f_arg_1 = raw.get_f_arg_1();
        this->f_arg_2 = raw.get_f_arg_2();
    };

    virtual bool isValid()
    {
        return false;
    } ///< \brief Return true if a command arguments are valid for that op code, false otherwise

    virtual void execute(){}; ///< \brief Executes Command Functionality or SFR Edit

    static uint16_t get_decimal_opcode(const uint8_t *hex_opcode_bytes)
    {
        return (hex_opcode_bytes[1] << 8) | (hex_opcode_bytes[0]);
    }

    static uint32_t get_decimal_arg(const uint8_t *hex_arg_bytes)
    {
        return (hex_arg_bytes[3] << 24) | (hex_arg_bytes[2]) << 16 | (hex_arg_bytes[1] << 8) | (hex_arg_bytes[0]);
    }
};

class SFROverrideCommand : public RockblockCommand
{
public:
    SFROverrideCommand(RawRockblockCommand raw) : RockblockCommand{raw}
    {
        if (SFRInterface::opcode_lookup.find(f_opcode) != SFRInterface::opcode_lookup.end()) {
            field = SFRInterface::opcode_lookup[f_opcode];
        }
    };

    void execute()
    {
        if (field) {
            field->setValue(f_arg_1);
        }
    }

    bool isValid()
    {
        return field != nullptr;
    }

private:
    SFRInterface *field;
};

class UnknownCommand : public RockblockCommand
{
public:
    UnknownCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};
};

#endif
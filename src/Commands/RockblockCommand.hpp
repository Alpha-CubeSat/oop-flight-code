#ifndef _ROCKBLOCKCOMMAND_HPP_
#define _ROCKBLOCKCOMMAND_HPP_

#include "SFRField.hpp"
#include <stdint.h>

class RawRockblockCommand
{
public:
    uint8_t opcode[2];
    uint8_t arg_1[4];
    uint8_t arg_2[4];
    uint16_t get_f_opcode();
    uint32_t get_f_arg_1();
    uint32_t get_f_arg_2();

    // added for checksum
    uint32_t get_checksum();
};

class RockblockCommand
{
public:
    const uint16_t f_opcode;
    const uint32_t f_arg_1;
    const uint32_t f_arg_2;
    RockblockCommand(RawRockblockCommand raw) : f_opcode{raw.get_f_opcode()}, f_arg_1{raw.get_f_arg_1()}, f_arg_2{raw.get_f_arg_2()} {};
    ~RockblockCommand(){};

    virtual bool isValid() { return false; }; ///< \brief Return true if a command arguments are valid for that op code, false otherwise
    virtual void execute(){};                 ///< \brief Executes Command Functionality or SFR Edit

    static uint16_t get_decimal_opcode(const uint8_t *hex_opcode_bytes)
    {
        return (hex_opcode_bytes[1] << 8) | (hex_opcode_bytes[0]);
    }

    static uint32_t get_decimal_arg(const uint8_t *hex_arg_bytes)
    {
        return (hex_arg_bytes[3] << 24) | (hex_arg_bytes[2]) << 16 | (hex_arg_bytes[1] << 8) | (hex_arg_bytes[0]);
    }

    static RockblockCommand commandFactory(RawRockblockCommand raw); ///< \brief Converts raw RB Command Data into a specific command type
};

class MissionModeCommand : public RockblockCommand
{
public:
    MissionModeCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};
    bool isValid();
    void execute();
};

class SFROverrideCommand : public RockblockCommand
{
public:
    SFROverrideCommand(RawRockblockCommand raw) : RockblockCommand{raw}
    {
        if (SFRInterface::opcode_lookup.find(f_arg_1) != SFRInterface::opcode_lookup.end()) {
            field = SFRInterface::opcode_lookup[f_arg_1];
        }
    };
    bool isValid();
    void execute();

private:
    SFRInterface *field;
};

class UnknownCommand : public RockblockCommand
{
public:
    UnknownCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};
};

#endif
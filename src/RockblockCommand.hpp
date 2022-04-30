#ifndef _ROCKBLOCKCOMMAND_HPP_
#define _ROCKBLOCKCOMMAND_HPP_

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
};

class RockblockCommand
{
public:
    uint16_t f_opcode;
    uint32_t f_arg_1;
    uint32_t f_arg_2;
    RockblockCommand(uint16_t f_opcode, uint32_t f_arg_1, uint32_t f_arg_2);
};

#endif
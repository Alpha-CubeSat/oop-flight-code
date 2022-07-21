#include "RockblockCommand.hpp"

uint16_t RawRockblockCommand::get_f_opcode()
{
    return (this->opcode[1] << 8) | (this->opcode[0]);
}

uint32_t RawRockblockCommand::get_f_arg_1()
{
    return (this->arg_1[3] << 24) | (this->arg_1[2] << 16) | (this->arg_1[1] << 8) | (this->arg_1[0]);
}

uint32_t RawRockblockCommand::get_f_arg_2()
{
    return (this->arg_2[3] << 24) | (this->arg_2[2] << 16) | (this->arg_2[1] << 8) | (this->arg_2[0]);
}

RockblockCommand::RockblockCommand(uint16_t f_opcode, uint32_t f_arg_1, uint32_t f_arg_2)
{
    this->f_opcode = f_opcode;
    this->f_arg_1 = f_arg_1;
    this->f_arg_2 = f_arg_2;
}
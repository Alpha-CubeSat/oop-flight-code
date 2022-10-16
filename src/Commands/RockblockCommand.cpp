#include "RockblockCommand.hpp"
#include "MissionManager.hpp"
#include "constants.hpp"
#include "sfr.hpp"

using namespace constants::rockblock;

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

bool RawRockblockCommand::check_checksum()
{
    // Get opcode and args, kinda
    uint16_t opcode = this->get_f_opcode();
    uint16_t arg_1_0 = (this->arg_1[1] << 8) | this->arg_1[0];
    uint16_t arg_1_1 = (this->arg_1[3] << 8) | this->arg_1[2];
    uint16_t arg_2_0 = (this->arg_2[1] << 8) | this->arg_2[0];
    uint16_t arg_2_1 = (this->arg_2[3] << 8) | this->arg_2[2];

    // Hash them to produce one 2-byte checksum
    uint16_t calculated_checksum = ((opcode & arg_1_0) ^ (arg_1_1 | arg_2_0)) & arg_2_1;

    // Compare to transmitted checksum
    return (calculated_checksum == (this->transmitted_checksum[1] << 8) | (this->transmitted_checksum[0]));
}

RockblockCommand RockblockCommand::commandFactory(RawRockblockCommand raw)
{
    // Create Specific Child Class of Rockblock command depending on the OP Code
    uint16_t op_code = raw.get_f_opcode();
    if (op_code == opcodes::mission_mode) {
        return MissionModeCommand(raw);
    } else if (op_code <= opcodes::sfr_field_opcode_max && op_code >= opcodes::sfr_field_opcode_min) {
        return SFROverrideCommand(raw);
    } else {
        return UnknownCommand(raw);
    }
}

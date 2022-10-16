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

// bool RawRockblockCommand::checksum()
// {
//     // Get opcode and args
//     uint32_t opcode = (uint32_t)(this->get_f_opcode());
//     uint32_t arg_1 = this->get_f_arg_1();
//     uint32_t arg_2 = this->get_f_arg_2();

//     return true;
// }

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

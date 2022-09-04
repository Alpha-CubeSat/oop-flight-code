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

RockblockCommand RockblockCommand::commandFactory(RawRockblockCommand raw)
{
    // Create Specific Child Class of Rockblock command depending on the OP Code
    uint16_t op_code = raw.get_f_opcode();
    if (op_code == opcodes::mission_mode) {
        return MissionModeCommand(raw);
    } else if (false) { // TODO After SFRField Merge
        return SFROverrideCommand(raw);
    } else if (op_code >= opcodes::fault_range_start && op_code < opcodes::fault_range_end) {
        return FaultCheckCommand(raw);
    } else {
        return UnknownCommand(raw);
    }
}

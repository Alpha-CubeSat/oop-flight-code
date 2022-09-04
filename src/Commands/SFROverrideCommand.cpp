#include "MissionManager.hpp"
#include "RockblockCommand.hpp"
#include "constants.hpp"
#include "sfr.hpp"

using namespace constants::rockblock;

void SFROverrideCommand::execute()
{
    if (field) {
        field->setValue(f_arg_1);
    }
}

bool SFROverrideCommand::isValid()
{
    return field != nullptr;
}
#include "sfr.hpp"

class DeployCommand : public RockblockCommand
{
public:
    DeployCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};
    DeployCommand(uint16_t f_opcode, uint32_t arg_1, uint32_t arg_2) : RockblockCommand(f_opcode, arg_1, arg_2){};
    void execute()
    {
        sfr::mission::current_mode = sfr::mission::normalDeployment;
    }

    bool isValid()
    {
        return sfr::mission::current_phase == sfr::mission::standby;
    }
};

class ArmCommand : public RockblockCommand
{
public:
    ArmCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};
    ArmCommand(uint16_t f_opcode, uint32_t arg_1, uint32_t arg_2) : RockblockCommand(f_opcode, arg_1, arg_2){};

    void execute()
    {
        sfr::mission::current_mode = sfr::mission::normalArmed;
    }

    bool isValid()
    {
        return sfr::mission::current_phase == sfr::mission::deployment;
    }
};

class FireCommand : public RockblockCommand
{
public:
    FireCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};
    FireCommand(uint16_t f_opcode, uint32_t arg_1, uint32_t arg_2) : RockblockCommand(f_opcode, arg_1, arg_2){};

    void execute()
    {
        sfr::mission::current_mode = sfr::mission::normalInSun;
    }

    bool isValid()
    {
        return sfr::mission::current_phase == sfr::mission::armed;
    }
};
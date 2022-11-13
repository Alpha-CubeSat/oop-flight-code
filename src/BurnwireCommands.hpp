#include "sfr.hpp"

class ArmCommand : public RockblockCommand
{
public:
    ArmCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};

    void execute()
    {
        sfr::burnwire::arm = f_arg_1;
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

    void execute()
    {
        sfr::burnwire::fire = f_arg_1;
    }

    bool isValid()
    {
        return sfr::mission::current_phase == sfr::mission::armed;
    }
};
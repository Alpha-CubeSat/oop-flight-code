#include "sfr.hpp"

class DeployCommand : public RockblockCommand
{
public:
    DeployCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};

    void execute()
    {
        sfr::mission::current_mode = sfr::mission::normalDeployment;
    }

    bool isValid()
    {
        return sfr::mission::current_phase->get_id() == sfr::mission::standby->get_id();
    }
};

class ArmCommand : public RockblockCommand
{
public:
    ArmCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};

    void execute()
    {
        sfr::mission::current_mode = sfr::mission::normalArmed;
    }

    bool isValid()
    {
        return sfr::mission::current_phase->get_id() == sfr::mission::deployment->get_id();
    }
};

class FireCommand : public RockblockCommand
{
public:
    FireCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};

    void execute()
    {
        sfr::mission::current_mode = sfr::mission::normalInSun;
    }

    bool isValid()
    {
        return sfr::mission::current_phase->get_id() == sfr::mission::armed->get_id();
    }
};
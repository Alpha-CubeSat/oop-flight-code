#include "RockblockCommand.hpp"
#include "sfr.hpp"

void CameraFragmentCommand::execute()
{
    sfr::camera::fragment_requested = true;
    sfr::camera::serial_requested = f_arg_1;
    sfr::camera::fragment_number_requested = f_arg_2;
}

bool CameraFragmentCommand::isValid()
{
    return sfr::mission::deployed;
}

void DeployCommand::execute()
{
    sfr::mission::current_mode = sfr::mission::normalDeployment;
}

bool DeployCommand::isValid()
{
    return sfr::mission::current_phase->get_id() == sfr::mission::standby->get_id();
}

void ArmCommand::execute()
{
    sfr::mission::current_mode = sfr::mission::normalArmed;
}

bool ArmCommand::isValid()
{
    return sfr::mission::current_phase->get_id() == sfr::mission::deployment->get_id();
}

void FireCommand::execute()
{
    sfr::mission::current_mode = sfr::mission::normalInSun;
}

bool FireCommand::isValid()
{
    return sfr::mission::current_phase->get_id() == sfr::mission::armed->get_id();
}

void MissionModeOverrideCommand::execute()
{
    switch (f_arg_1) {
    case 0: {
        sfr::mission::current_mode = sfr::mission::boot;
        break;
    }
    case 1: {
        sfr::mission::current_mode = sfr::mission::aliveSignal;
        break;
    }
    case 2: {
        sfr::mission::current_mode = sfr::mission::detumbleSpin;
        break;
    }
    case 3: {
        sfr::mission::current_mode = sfr::mission::lowPowerDetumbleSpin;
        break;
    }
    case 4: {
        sfr::mission::current_mode = sfr::mission::normal;
        break;
    }
    case 5: {
        sfr::mission::current_mode = sfr::mission::transmit;
        break;
    }
    case 6: {
        sfr::mission::current_mode = sfr::mission::lowPower;
        break;
    }
    case 7: {
        sfr::mission::current_mode = sfr::mission::normalDeployment;
        break;
    }
    case 8: {
        sfr::mission::current_mode = sfr::mission::transmitDeployment;
        break;
    }
    case 9: {
        sfr::mission::current_mode = sfr::mission::lowPowerDeployment;
        break;
    }
    case 10: {
        sfr::mission::current_mode = sfr::mission::normalArmed;
        break;
    }
    case 11: {
        sfr::mission::current_mode = sfr::mission::transmitArmed;
        break;
    }
    case 12: {
        sfr::mission::current_mode = sfr::mission::lowPowerArmed;
        break;
    }
    case 13: {
        sfr::mission::current_mode = sfr::mission::normalInSun;
        break;
    }
    case 14: {
        sfr::mission::current_mode = sfr::mission::transmitInSun;
        break;
    }
    case 15: {
        sfr::mission::current_mode = sfr::mission::lowPowerInSun;
        break;
    }
    case 16: {
        sfr::mission::current_mode = sfr::mission::voltageFailureInSun;
        break;
    }
    case 17: {
        sfr::mission::current_mode = sfr::mission::bootIMU;
        break;
    }
    case 18: {
        sfr::mission::current_mode = sfr::mission::bootCamera;
        break;
    }
    case 19: {
        sfr::mission::current_mode = sfr::mission::mandatoryBurns;
        break;
    }
    case 20: {
        sfr::mission::current_mode = sfr::mission::regularBurns;
        break;
    }
    case 21: {
        sfr::mission::current_mode = sfr::mission::photo;
        break;
    }
    }
}

bool MissionModeOverrideCommand::isValid()
{
    return (f_arg_1 >= sfr::mission::boot->get_id() && f_arg_1 <= sfr::mission::photo->get_id());
}
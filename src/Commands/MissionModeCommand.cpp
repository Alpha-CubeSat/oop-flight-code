#include "MissionManager.hpp"
#include "RockblockCommand.hpp"
#include "constants.hpp"
#include "sfr.hpp"

using namespace constants::rockblock;

void MissionModeCommand::execute()
{
    // Set Mission Phase
    std::map<uint32_t, MissionMode *> mode_by_op;
    mode_by_op[args::BOOT] = sfr::mission::boot;
    mode_by_op[args::ALIVE_SIGNAL] = sfr::mission::aliveSignal;
    mode_by_op[args::LOW_POWER_ALIVE_SIGNAL] = sfr::mission::lowPowerAliveSignal;
    mode_by_op[args::DETUMBLE_SPIN] = sfr::mission::detumbleSpin;
    mode_by_op[args::LOW_POWER_DETUMBLE_SPIN] = sfr::mission::lowPowerDetumbleSpin;
    mode_by_op[args::NORMAL] = sfr::mission::normal;
    mode_by_op[args::TRANSMIT] = sfr::mission::transmit;
    mode_by_op[args::LOW_POWER] = sfr::mission::lowPower;
    mode_by_op[args::NORMAL_DEPLOYMENT] = sfr::mission::normalDeployment;
    mode_by_op[args::TRANSMIT_DEPLOYMENT] = sfr::mission::transmitDeployment;
    mode_by_op[args::LOW_POWER_DEPLOYMENT] = sfr::mission::lowPowerDeployment;
    mode_by_op[args::NORMAL_IN_SUN] = sfr::mission::normalInSun;
    mode_by_op[args::TRANSMIT_IN_SUN] = sfr::mission::transmitInSun;
    mode_by_op[args::LOW_POWER_IN_SUN] = sfr::mission::lowPowerInSun;
    mode_by_op[args::VOLT_FAIL_IN_SUN] = sfr::mission::voltageFailureInSun;
    mode_by_op[args::BOOT_CAMERA] = sfr::mission::bootCamera;
    mode_by_op[args::MANDATORY_BURNS] = sfr::mission::mandatoryBurns;
    mode_by_op[args::REGULAR_BURNS] = sfr::mission::regularBurns;
    mode_by_op[args::PHOTO] = sfr::mission::photo;

    if (mode_by_op.find(f_arg_1) != mode_by_op.end()) {
        sfr::mission::current_mode = mode_by_op[f_arg_1];
    }
}

bool MissionModeCommand::isValid()
{
    return f_arg_1 >= args::MISSION_MODE_MIN && f_arg_1 <= args::MISSION_MODE_MAX;
}
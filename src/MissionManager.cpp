#include "MissionManager.hpp"

MissionManager::MissionManager(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    boot_initialization();
}

void MissionManager::execute()
{
    sfr::mission::current_phase = modeToPhase(sfr::mission::current_mode);

    if (sfr::mission::previous_phase->get_id() != sfr::mission::current_phase->get_id()) {
        sfr::mission::current_phase->set_start_time(millis());
    }

    if (sfr::mission::current_phase->get_id() == sfr::mission::armed->get_id() && (millis() - sfr::mission::current_phase->start_time) > sfr::burnwire::armed_time) {
        sfr::mission::current_mode = sfr::mission::normalDeployment;
        sfr::mission::current_phase = modeToPhase(sfr::mission::current_mode);
    }

    if (sfr::mission::previous_mode->get_id() != sfr::mission::current_mode->get_id()) {
        sfr::mission::current_mode->set_start_time(millis());
        sfr::mission::current_mode->transition_to();
#ifdef E2E_TESTING
        Serial.print("Setting mode to ");
        Serial.println(sfr::mission::current_mode->get_id());
#endif
        sfr::mission::mode_history.push_front(sfr::mission::current_mode->get_id());
    }

    sfr::mission::previous_mode = sfr::mission::current_mode;
    sfr::mission::previous_phase = modeToPhase(sfr::mission::current_mode);
    sfr::mission::current_mode->dispatch();
}

Phase *MissionManager::modeToPhase(MissionMode *mission)
{
    if (mission->get_id() == sfr::mission::detumbleSpin->get_id() || mission->get_id() == sfr::mission::lowPowerDetumbleSpin->get_id()) {
        return sfr::mission::stabilization;
    } else if (mission->get_id() == sfr::mission::normal->get_id() || mission->get_id() == sfr::mission::transmit->get_id() || mission->get_id() == sfr::mission::lowPower->get_id()) {
        return sfr::mission::standby;
    } else if (mission->get_id() == sfr::mission::normalDeployment->get_id() || mission->get_id() == sfr::mission::transmitDeployment->get_id() || mission->get_id() == sfr::mission::lowPowerDeployment->get_id()) {
        return sfr::mission::deployment;
    } else if (mission->get_id() == sfr::mission::normalArmed->get_id() || mission->get_id() == sfr::mission::transmitArmed->get_id() || mission->get_id() == sfr::mission::lowPowerArmed->get_id()) {
        return sfr::mission::armed;
    } else if (mission->get_id() == sfr::mission::normalInSun->get_id() || mission->get_id() == sfr::mission::transmitInSun->get_id() || mission->get_id() == sfr::mission::lowPowerInSun->get_id() || mission->get_id() == sfr::mission::voltageFailureInSun->get_id()) {
        return sfr::mission::inSun;
    } else if (mission->get_id() == sfr::mission::bootCamera->get_id() || mission->get_id() == sfr::mission::mandatoryBurns->get_id() || mission->get_id() == sfr::mission::regularBurns->get_id() || mission->get_id() == sfr::mission::photo->get_id()) {
        return sfr::mission::firing;
    } else {
        return sfr::mission::initialization;
    }
}

#include "MissionManager.hpp"

MissionManager::MissionManager(unsigned int offset) : TimedControlTask<void>(offset)
{
    boot_initialization();
    sfr::mission::boot->set_id(0);
    sfr::mission::aliveSignal->set_id(1);
    sfr::mission::lowPowerAliveSignal->set_id(2);
    sfr::mission::detumbleSpin->set_id(3);
    sfr::mission::lowPowerDetumbleSpin->set_id(4);
    sfr::mission::normal->set_id(5);
    sfr::mission::transmit->set_id(6);
    sfr::mission::lowPower->set_id(7);
    sfr::mission::normalDeployment->set_id(8);
    sfr::mission::transmitDeployment->set_id(9);
    sfr::mission::lowPowerDeployment->set_id(10);
    sfr::mission::normalArmed->set_id(11);
    sfr::mission::transmitArmed->set_id(12);
    sfr::mission::lowPowerArmed->set_id(13);
    sfr::mission::normalInSun->set_id(14);
    sfr::mission::transmitInSun->set_id(15);
    sfr::mission::lowPowerInSun->set_id(16);
    sfr::mission::voltageFailureInSun->set_id(17);
    sfr::mission::bootCamera->set_id(18);
    sfr::mission::mandatoryBurns->set_id(19);
    sfr::mission::regularBurns->set_id(20);
    sfr::mission::photo->set_id(21);
}

void MissionManager::execute()
{
    if (sfr::mission::previous_mode->id != sfr::mission::current_mode->id) {
        sfr::mission::current_mode->transition_to();
        sfr::mission::mode_history.push_front(sfr::mission::current_mode->id);
    }

    sfr::mission::current_mode->dispatch();

    sfr::mission::previous_mode = sfr::mission::current_mode;
}

#include "MissionManager.hpp"

MissionManager::MissionManager(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    boot_initialization();
}

void MissionManager::execute()
{
    if (sfr::mission::previous_mode->get_id() != sfr::mission::current_mode->get_id()) {
        sfr::mission::current_mode->transition_to();
        sfr::mission::mode_history.push_front(sfr::mission::current_mode->get_id()); 
    }

    sfr::mission::current_mode->dispatch();

    sfr::mission::previous_mode = sfr::mission::current_mode;
}

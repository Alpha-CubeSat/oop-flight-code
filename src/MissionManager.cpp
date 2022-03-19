#include "MissionManager.hpp"

MissionManager::MissionManager(unsigned int offset) : TimedControlTask<void>(offset)
{
    sfr::mission::boot->transition_to();
}

void MissionManager::execute()
{
    if (sfr::mission::previous_mode->id() != sfr::mission::current_mode->id()) {
        sfr::mission::current_mode->transition_to();
    }

    sfr::mission::current_mode->dispatch();

    sfr::mission::previous_mode = sfr::mission::current_mode;
}

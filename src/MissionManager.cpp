#include "MissionManager.hpp"

MissionManager::MissionManager()
{
}

void MissionManager::execute()
{
    if (first) {
        boot_initialization();
        first = false;
    }

    if (sfr::mission::previous_phase->get_id() != sfr::mission::current_mode->get_phase()->get_id()) {
        sfr::mission::current_mode->get_phase()->set_start_time(millis());
    }

    if (sfr::mission::previous_mode->get_id() != sfr::mission::current_mode->get_id()) {
        sfr::mission::current_mode->set_start_time(millis());
        sfr::mission::current_mode->transition_to();
        if (sfr::mission::mode_history.size() >= constants::rockblock::mission_mode_hist_length) {
            sfr::mission::mode_history.pop_back();
        }
        sfr::mission::mode_history.push_front(sfr::mission::current_mode->get_id());
    }

    sfr::mission::previous_mode = sfr::mission::current_mode;
    sfr::mission::previous_phase = sfr::mission::current_mode->get_phase();
    sfr::mission::current_mode->dispatch();
}

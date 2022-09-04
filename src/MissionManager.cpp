#include "MissionManager.hpp"

MissionManager::MissionManager(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    boot_initialization();
}

void MissionManager::execute()
{
    if (previous_mode.get_id() != current_mode.get_id()) {
        current_mode.set_start_time(millis());
        current_mode.transition_to();
        mode_history.push_front(current_mode.get_id());
    }

    if (previous_phase.get_id() != current_phase.get_id()) {
        current_phase.set_start_time(millis());
    }

    previous_mode = current_mode;
    current_mode.dispatch();
}

//Taken from Cornell's PAN

#include "ClockManager.hpp"

ClockManager::ClockManager(const unsigned int _control_cycle_size) :
    TimedControlTask<void>(0),
    control_cycle_size(_control_cycle_size)
{
    initial_start_cycling_time = get_system_time();
}

void ClockManager::execute() {
    unsigned int dt = 0;
    if (has_executed) {
        sys_time_t earliest_start_time =
            TimedControlTaskBase::control_cycle_start_time + control_cycle_size;
        dt = wait_until_time(earliest_start_time);
    }
    has_executed = true;
    TimedControlTaskBase::control_cycle_start_time = get_system_time();
    control_cycle_count++;
    control_cycle_count_f=control_cycle_count;
}

unsigned int ClockManager::systime_to_cycle(sys_time_t time) {
    systime_duration_t duration = time - initial_start_cycling_time;
    return duration/control_cycle_size;
}

sys_time_t ClockManager::cycle_to_systime(unsigned int ccno) {
    systime_duration_t duration = ccno * control_cycle_size;
    return initial_start_cycling_time+duration;
}
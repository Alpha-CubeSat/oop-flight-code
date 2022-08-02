#ifndef MISSION_MANAGER_HPP_
#define MISSION_MANAGER_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "MissionMode.hpp"
#include "Phase.hpp"
#include "constants.hpp"
#include <deque>

class MissionManager : public TimedControlTask<void>
{
public:
    MissionManager(unsigned int offset);
    void execute();
    std::deque<int> mode_history;

    Boot boot;
    Phase initialization;

    MissionMode current_mode = boot;
    MissionMode previous_mode = boot;

    Phase current_phase = initialization;
    Phase previous_phase = initialization;
};

#endif
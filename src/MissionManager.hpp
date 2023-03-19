#ifndef MISSION_MANAGER_HPP_
#define MISSION_MANAGER_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "MissionMode.hpp"
#include "Phase.hpp"
#include "constants.hpp"
#include "sfr.hpp"
#include <deque>

class MissionManager : public TimedControlTask<void>
{
public:
    MissionManager(unsigned int offset);
    Phase *modeToPhase(MissionMode *mission);
    void execute();
};

#endif
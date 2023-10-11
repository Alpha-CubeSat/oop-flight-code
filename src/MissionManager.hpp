#ifndef MISSION_MANAGER_HPP_
#define MISSION_MANAGER_HPP_

#include "MissionMode.hpp"
#include "Phase.hpp"
#include "constants.hpp"
#include "sfr.hpp"
#include <deque>

class MissionManager 
{
public:
    MissionManager();
    Phase *modeToPhase(MissionMode *mission);
    void execute();

private:
    bool first = true;
};

#endif
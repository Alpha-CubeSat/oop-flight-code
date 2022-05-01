#ifndef MISSION_MANAGER_HPP_
#define MISSION_MANAGER_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "MissionMode.hpp"
#include "constants.hpp"
#include "sfr.hpp"

class MissionManager : public TimedControlTask<void>
{
public:
    MissionManager(unsigned int offset);
    void execute();
};

#endif
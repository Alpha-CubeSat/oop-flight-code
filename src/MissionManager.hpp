#ifndef MISSION_MANAGER_HPP_
#define MISSION_MANAGER_HPP_

#include "constants.hpp"
#include "sfr.hpp"
#include <deque>

class MissionManager
{
public:
    MissionManager();
    void execute();

private:
    bool first = true;
};

#endif
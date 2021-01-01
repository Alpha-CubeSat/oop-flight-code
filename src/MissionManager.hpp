#ifndef MISSION_MANAGER_HPP_
#define MISSION_MANAGER_HPP_

#include "sfr.hpp"
#include "constants.hpp"

class MissionManager{
    public:
        MissionManager();
        void execute();
        void dispatch_normal();
        void dispatch_detumble();
        void dispatch_safe_hold();
        void dispatch_low_power();
        void dispatch_deploy();
};

#endif
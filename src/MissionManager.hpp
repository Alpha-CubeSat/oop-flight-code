#ifndef MISSION_MANAGER_HPP_
#define MISSION_MANAGER_HPP_

#include "sfr.hpp"
#include "constants.hpp"

class MissionManager{
    public:
        MissionManager();
        void execute();
        void dispatch_standby();
        void dispatch_safe();
        void dispatch_low_power();
        void dispatch_deployment();

        void transition_to_standby();
        void transition_to_safe();
        void transition_to_low_power();
        void transition_to_deployment();
};

#endif
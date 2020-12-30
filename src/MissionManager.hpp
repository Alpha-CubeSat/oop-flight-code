#ifndef MISSION_MANAGER_HPP_
#define MISSION_MANAGER_HPP_

#include "sfr.hpp"
#include "constants.hpp"

class MissionManager{
    public:
        MissionManager();
        void execute();
        void dispatch_ascent();
        void dispatch_deploy_lightsail();
        void dispatch_take_photo();
        void dispatch_descent();
};

#endif
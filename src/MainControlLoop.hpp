#ifndef MAIN_CONTROL_LOOP_HPP_
#define MAIN_CONTROL_LOOP_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include "PhotoresistorMonitor.hpp"
#include "MissionManager.hpp"
#include "BurnwireControlTask.hpp"
#include "CameraControlTask.hpp"

class MainControlLoop{
    protected:
        PhotoresistorMonitor photoresistor_monitor;
        

        //MissionManager mission_manager;

        BurnwireControlTask burnwire_control_task;
        CameraControlTask camera_control_task;
    
    public:
        MainControlLoop();
        void execute();
};

#endif
#ifndef CAMERA_CONTROL_TASK_HPP_
#define CAMERA_CONTROL_TASK_HPP_

#include <SD.h>
#include <Adafruit_VC0706.h>
#include "sfr.hpp"
#include "constants.hpp"

class CameraControlTask{

    public:
        CameraControlTask();
        void execute();
        Adafruit_VC0706 adaCam;
       
};

#endif
#ifndef CAMERA_CONTROL_TASK_HPP_
#define CAMERA_CONTROL_TASK_HPP_

#ifndef GIT
#include <SD.h>
#include <Adafruit_VC0706.h>
#endif

#include "sfr.hpp"
#include "constants.hpp"

class CameraControlTask{

    public:
        CameraControlTask();
        void execute();
        #ifndef GIT
        Adafruit_VC0706 adaCam;
        #endif

        //move to sfr
        char filename[13];
        uint16_t jpglen = 0;
       
};



#endif
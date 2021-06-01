#ifndef CAMERA_CONTROL_TASK_HPP_
#define CAMERA_CONTROL_TASK_HPP_

#include "sfr.hpp"

class CameraControlTask : public TimedControlTask<void>
{

public:
    CameraControlTask(unsigned int offset);
    void execute();
    Adafruit_VC0706 adaCam;

    //move to sfr
    char filename[13];
    uint16_t jpglen = 0;
};

#endif
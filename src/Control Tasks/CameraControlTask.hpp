#ifndef CAMERA_CONTROL_TASK_HPP_
#define CAMERA_CONTROL_TASK_HPP_

#include "Arduino.h"
#include "Pins.hpp"
#include "sfr.hpp"
#include <SD.h>

class CameraControlTask
{

public:
    CameraControlTask();
    void execute();

private:
    void camera_init();
    void camera_shutdown();
    void transition_to_normal();
    void transition_to_abnormal_init();
    uint16_t jpglen = 0;
    Adafruit_VC0706 adaCam;
};

#endif
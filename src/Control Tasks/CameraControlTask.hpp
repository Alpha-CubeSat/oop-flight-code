#ifndef CAMERA_CONTROL_TASK_HPP_
#define CAMERA_CONTROL_TASK_HPP_

#include "Arduino.h"
#include "Control Tasks/TimedControlTask.hpp"
#include "Pins.hpp"
#include "sfr.hpp"
#include <SD.h>

class CameraControlTask : public TimedControlTask<void>
{

public:
    CameraControlTask(unsigned int offset);
    void execute();
    Adafruit_VC0706 adaCam;
    File imgFile;
    String filetocreate;

private:
    void camera_init();
    void transition_to_normal();
    void transition_to_abnormal_init();
    void transition_to_retry();
    uint16_t jpglen = 0;
    int image_lengths[255];
    char filename[15];
    int camera_max_fragments[99] = {};
};

#endif
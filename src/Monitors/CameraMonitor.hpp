#ifndef CAMERA_MONITOR_HPP_
#define CAMERA_MONITOR_HPP_

#include "ISensorMonitor.hpp"
#include "sfr.hpp"

class CameraMonitor : public TimedControlTask<void>, public ISensorMonitor
{
public:
    CameraMonitor(unsigned int offset);
    void execute();
    Adafruit_VC0706 adaCam;

    void transition_to_normal();
    void transition_to_abnormal_init();
    void transition_to_retry();
};

#endif
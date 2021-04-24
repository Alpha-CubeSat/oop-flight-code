#ifndef CAMERA_REPORT_MONITOR_HPP_
#define CAMERA_REPORT_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <SD.h>
#include "Control Tasks/TimedControlTask.hpp"

class CameraReportMonitor : public TimedControlTask<void>
{
public:
    CameraReportMonitor(unsigned int offset);
    void execute();
    File imgFile;
};

#endif
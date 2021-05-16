#ifndef CAMERA_REPORT_MONITOR_HPP_
#define CAMERA_REPORT_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#ifndef GIT
#include <SD.h>
#endif
#include "Control Tasks/TimedControlTask.hpp"
#ifndef GIT
class CameraReportMonitor : public TimedControlTask<void>
{
public:
    CameraReportMonitor(unsigned int offset);
    void execute();
    File imgFile;
};
#else
class CameraReportMonitor : public TimedControlTask<void>
{
public:
    CameraReportMonitor(unsigned int offset);
    void execute();
};
#endif
#endif
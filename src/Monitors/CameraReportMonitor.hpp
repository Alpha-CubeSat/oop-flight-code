#ifndef CAMERA_REPORT_MONITOR_HPP_
#define CAMERA_REPORT_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <Arduino.h>


class CameraReportMonitor{
    public:
        CameraReportMonitor();
        void execute();
};

#endif
#ifndef CAMERA_REPORT_MONITOR_HPP_
#define CAMERA_REPORT_MONITOR_HPP_

#include "Control Tasks/RockblockControlTask.hpp"
#include "SD.h"
#include "sfr.hpp"
#include <vector>

class CameraReportMonitor
{
public:
    CameraReportMonitor();
    void execute();

private:
    void create_camera_report(int fragment_number, uint8_t serial_number);
    uint32_t fragment_number = 0;
    uint32_t current_serial = 0;
    File imgFile;
};
#endif

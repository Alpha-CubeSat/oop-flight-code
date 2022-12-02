#ifndef CAMERA_REPORT_MONITOR_HPP_
#define CAMERA_REPORT_MONITOR_HPP_

#include "Control Tasks/RockblockControlTask.hpp"
#include "Control Tasks/TimedControlTask.hpp"
#include "SD.h"
#include "sfr.hpp"
#include <vector>

class CameraReportMonitor : public TimedControlTask<void>
{
public:
    CameraReportMonitor(unsigned int offset);
    void execute();
    File imgFile;
    void create_camera_report(int fragment_number, uint8_t serial_number);
    bool fragment_requested;
    uint32_t fragment_number_requested;
    uint8_t serial_requested;
    bool images_written = 0;
    bool full_image_written = false;
    uint32_t fragment_number = 0;
    uint32_t current_serial;
};
#endif

#ifndef CAMERA_REPORT_MONITOR_HPP_
#define CAMERA_REPORT_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "SD.h"
#include <vector>
#include "sfr.hpp"

class CameraReportMonitor : public TimedControlTask<void>
{
public:
    CameraReportMonitor(unsigned int offset);
    void execute();
    File imgFile;
    void create_camera_report(int fragment_number, uint8_t serial_number);
    void add_possible_command();

private:
    boolean report_downlinked;
    boolean fragment_requested;
    uint32_t fragment_number_requested;
    uint8_t serial_requested;
    boolean images_written;
    boolean full_image_written;
    uint32_t fragment_number;
    boolean report_ready;
    uint32_t current_serial;
};
#endif

#ifndef IMU_DOWNLINK_REPORT_MONITOR_HPP_
#define IMU_DOWNLINK_REPORT_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include <cassert>
#include <vector>
#include "sfr.hpp"

class IMUDownlinkReportMonitor : public TimedControlTask<void>
{
public:
    IMUDownlinkReportMonitor(unsigned int offset);
    void execute();
    void create_imu_downlink_report(int fragment_number);
    bool start_timing_deployed = false;
    uint8_t current_sample = 0;
    bool sample_gyro = false;
    uint8_t fragment_number = 0;
    bool report_ready = false;
    bool report_downlinked = true;
    bool report_written = false;
    bool full_report_written = false;
};

#endif
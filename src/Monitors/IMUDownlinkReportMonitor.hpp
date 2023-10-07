#ifndef IMU_DOWNLINK_REPORT_MONITOR_HPP_
#define IMU_DOWNLINK_REPORT_MONITOR_HPP_

#include "sfr.hpp"
#include <SD.h>
#include <cassert>
#include <vector>

class IMUDownlinkReportMonitor
{
public:
    IMUDownlinkReportMonitor();
    void execute();

private:
    void create_imu_downlink_report(uint8_t fragment_number);
    void create_imu_downlink_report_from_SD(uint8_t fragment_number);
    bool start_timing_deployed = false;
    uint8_t current_sample = 0;
    uint8_t fragment_number = 0;
    bool report_ready = false;
    bool report_downlinked = true;
};

#endif
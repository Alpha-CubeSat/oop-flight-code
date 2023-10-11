#ifndef IMU_DOWNLINK_REPORT_MONITOR_HPP_
#define IMU_DOWNLINK_REPORT_MONITOR_HPP_

#include "sfr.hpp"
#include <cassert>
#include <vector>

class IMUDownlinkReportMonitor
{
public:
    IMUDownlinkReportMonitor();
    void execute();

private:
    void create_imu_downlink_report(int fragment_number);
    bool start_timing_deployed = false;
    uint8_t current_sample = 0;
    uint8_t fragment_number = 0;
    bool report_ready = false;
    bool report_downlinked = true;
    // Sets the amount of values that go into the report.
    int pop_size = min(constants::imu::max_gyro_imu_report_size, sfr::imu::imu_dlink.size());
};

#endif
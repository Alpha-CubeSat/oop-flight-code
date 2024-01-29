#ifndef IMU_DOWNLINK_REPORT_MONITOR_HPP_
#define IMU_DOWNLINK_REPORT_MONITOR_HPP_

#include "sfr.hpp"
#include <vector>

class IMUDownlinkReportMonitor
{
public:
    IMUDownlinkReportMonitor();
    void execute();

private:
    void create_imu_downlink_report(uint8_t fragment_number);
    uint8_t fragment_number = 0;
};

#endif
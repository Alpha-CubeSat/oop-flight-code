#ifndef IMU_DOWNLINK_REPORT_MONITOR_HPP_
#define IMU_DOWNLINK_REPORT_MONITOR_HPP_

#include "IMUDownlink.hpp"
#include "sfr.hpp"
#include <cassert>
#include <vector>

class IMUDownlinkReportMonitor : public TimedControlTask<void>
{
public:
    IMUDownlinkReportMonitor(unsigned int offset);
    void execute();
    void create_imu_downlink_report(int fragment_number);
};

#endif
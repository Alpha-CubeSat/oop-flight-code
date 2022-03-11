#ifndef IMU_DOWNLINK_REPORT_MONITOR_HPP_
#define IMU_DOWNLINK_REPORT_MONITOR_HPP_

#include "IMUDownlink.hpp"
#include "sfr.hpp"
#include <vector>

class IMUDownlinkReportMonitor : public TimedControlTask<void>
{
public:
    IMUDownlinkReportMonitor(unsigned int offset);
    void execute();
    void IMUDownlinkReportMonitor::create_imu_downlink_report(int fragment_number);
};

#endif
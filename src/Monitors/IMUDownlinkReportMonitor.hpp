#ifndef IMU_DOWNLINK_REPORT_MONITOR_HPP_
#define IMU_DOWNLINK_REPORT_MONITOR_HPP_

#include "IMUDownlink.hpp"
#include "sfr.hpp"

class IMUDownlinkReportMonitor : public TimedControlTask<void>
{
public:
    IMUDownlinkReportMonitor(unsigned int offset);
    void execute();
};

#endif
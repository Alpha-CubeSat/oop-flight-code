#ifndef IMU_DOWNLINK_HPP_
#define IMU_DOWNLINK_HPP_

#include "NormalReportMonitor.hpp"
#include "sfr.hpp"

class IMUDownlink
{
public:
    IMUDownlink();
    void execute();
};

#endif
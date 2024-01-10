#ifndef IMU_DOWNLINK_HPP_
#define IMU_DOWNLINK_HPP_

#include "Adafruit_LSM9DS1.h"
#include "NormalReportMonitor.hpp"
#include "sfr.hpp"

class IMUDownlink
{
public:
    IMUDownlink();
    void execute();

private:
    Adafruit_LSM9DS1 imu;
};

#endif
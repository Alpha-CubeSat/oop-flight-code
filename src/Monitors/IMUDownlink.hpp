#ifndef IMU_DOWNLINK_HPP_
#define IMU_DOWNLINK_HPP_
#include "sfr.hpp"

class IMUDownlink : public TimedControlTask<void>
{
public:
    IMUDownlink(unsigned int offset);
    void execute();
    Adafruit_LSM9DS1 imu;
};

#endif
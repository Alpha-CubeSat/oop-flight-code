#ifndef IMU_DOWNLINK_HPP_
#define IMU_DOWNLINK_HPP_

#include "Adafruit_LSM9DS1.h"
#include "Control Tasks/TimedControlTask.hpp"
#include "sfr.hpp"

class IMUDownlink : public TimedControlTask<void>
{
public:
    IMUDownlink(unsigned int offset);
    void execute();
    Adafruit_LSM9DS1 imu;
    std::deque<uint8_t> imu_dlink;
};

#endif
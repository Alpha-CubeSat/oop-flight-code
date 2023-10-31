#ifndef IMU_DOWNLINK_HPP_
#define IMU_DOWNLINK_HPP_

#include "Adafruit_LSM9DS1.h"
#include "sfr.hpp"
#include <SD.h>

class IMUDownlink
{
public:
    IMUDownlink();
    void execute();

private:
    Adafruit_LSM9DS1 imu;
    uint8_t fragment_number = 0;
    uint32_t values_unwritten;
    uint32_t start_index = 0;
};

#endif
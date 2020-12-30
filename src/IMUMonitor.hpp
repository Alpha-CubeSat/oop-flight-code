#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include <Adafruit_LSM9DS1.h>
#include "sfr.hpp"
#include <Arduino.h>

class IMUMonitor{
    public:
        IMUMonitor();
        void execute();
        Adafruit_LSM9DS1 imu;
};

#endif
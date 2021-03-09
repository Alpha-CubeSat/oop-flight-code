#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#ifndef GIT
#include <Adafruit_LSM9DS1.h>
#endif
#include "sfr.hpp"
#include "constants.hpp"

class IMUMonitor{
    public:
        IMUMonitor();
        void execute();
        #ifndef GIT
        Adafruit_LSM9DS1 imu;
        #endif
};

#endif
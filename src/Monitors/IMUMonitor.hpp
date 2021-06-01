#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "sfr.hpp"

class IMUMonitor : public TimedControlTask<void>{
    public:
        IMUMonitor(unsigned int offset);
        void execute();
        Adafruit_LSM9DS1 imu;
};

#endif
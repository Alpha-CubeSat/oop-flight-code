#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "sfr.hpp"

extern uint32_t start;
extern uint32_t stop;
extern bool began;

class IMUMonitor : public TimedControlTask<void>{
    public:
        IMUMonitor(unsigned int offset);
        void execute();
        Adafruit_LSM9DS1 imu;
};

#endif
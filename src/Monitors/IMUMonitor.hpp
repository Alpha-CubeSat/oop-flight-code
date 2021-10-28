#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "ISensorMonitor.hpp"
#include "sfr.hpp"

extern uint32_t start;
extern uint32_t stop;
extern bool began;

class IMUMonitor : public TimedControlTask<void>, public ISensorMonitor{
    public:
        IMUMonitor(unsigned int offset);
        void execute();
        Adafruit_LSM9DS1 imu;

        void transition_to_normal();
        void transition_to_abnormal();
        void transition_to_abandon();
};

#endif
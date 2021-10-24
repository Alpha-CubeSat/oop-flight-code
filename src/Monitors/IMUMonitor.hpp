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

        static void transition_to_normal();
        static void transition_to_abnormal();
        static void transition_to_abandon();
};

#endif
#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "sfr.hpp"

class IMUMonitor : public TimedControlTask<void>
{
public:
    IMUMonitor(unsigned int offset);
    void execute();
    Adafruit_LSM9DS1 imu;
    File imuFile;

    void transition_to_normal();
    void transition_to_abnormal_init();
    void transition_to_retry();

private:
    void capture_imu_values();
};

#endif
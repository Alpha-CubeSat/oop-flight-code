#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "TimedControlTask.hpp"
#include <Adafruit_LSM9DS1.h>

class IMUMonitor : public TimedControlTask<void> {

    public:
        IMUMonitor(StateFieldRegistry &registry, unsigned int offset);
        Adafruit_LSM9DS1 imu = Adafruit_LSM9DS1();
        void execute() override;
    
    protected:
        InternalStateField<float> mag_x, mag_y, mag_z, gyro_x, gyro_z, gyro_y;
};

#endif
#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "Adafruit_LSM9DS1.h"
#include "Control Tasks/TimedControlTask.hpp"
#include "Modes/sensor_mode_type.enum"
#include "SensorReading.hpp"
#include "Pins.hpp"
#include "sfr.hpp"
class IMUMonitor : public TimedControlTask<void>
{
public:
    IMUMonitor(unsigned int offset);
    void execute();
    Adafruit_LSM9DS1 imu;

private:
    void IMU_setup();
    void IMU_init();
    void transition_to_normal();
    void transition_to_abnormal_init();
    void transition_to_retry();
    void capture_imu_values();

    sensor_mode_type mode;
};

#endif
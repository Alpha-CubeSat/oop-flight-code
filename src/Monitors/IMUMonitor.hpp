#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "Adafruit_LSM9DS1.h"
#include "Control Tasks/TimedControlTask.hpp"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "Pins.hpp"
#include "SensorReading.hpp"
#include "sfr.hpp"
class IMUMonitor : public TimedControlTask<void>
{
public:
    IMUMonitor(unsigned int offset);
    void execute();

private:
    void IMU_init();
    void transition_to_normal();
    void transition_to_abnormal_init();
    void capture_imu_values();
    void invalidate_data();

    sensor_mode_type mode;

    Adafruit_LSM9DS1 imu;
};

#endif
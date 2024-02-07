#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "Adafruit_LSM9DS1.h"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "Pins.hpp"
#include "SensorReading.hpp"
#include "ekf.h"
#include "sfr.hpp"
class IMUMonitor
{
public:
    EKF ekfObj;
    IMUMonitor();
    void execute();

private:
    void IMU_init();
    void invalidate_data();
    void transition_to_normal();
    void transition_to_abnormal_init();
    void capture_imu_values();
    void imu_offset();
    Adafruit_LSM9DS1 imu;

    bool first=true;
};

#endif
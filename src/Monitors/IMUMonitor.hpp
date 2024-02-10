#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "Adafruit_LSM9DS1.h"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "Pins.hpp"
#include "SensorReading.hpp"
#include "ekf.h"
#include "sfr.hpp"

#ifdef ACS_SIM
#include "Plant.h"
#endif
class IMUMonitor
{
public:
    EKF ekfObj;
#ifdef ACS_SIM
    Plant plantObj;
#endif

    IMUMonitor();
    void execute();

private:
    void IMU_init();
    void invalidate_data();
    void transition_to_normal();
    void transition_to_abnormal_init();
    void capture_imu_values();
    void imu_offset();

    float temp;
    float voltage;

    float mag_x;
    float mag_y;
    float mag_z;

    float gyro_x;
    float gyro_y;
    float gyro_z;

    Adafruit_LSM9DS1 imu;

    bool first = true;

#ifdef ACS_SIM
    double altitude_input = 400;
    double I_input[9] = {0.00195761450869, -5.836632382E-5, 2.27638093E-6,
                         -5.836632382E-5, 0.00196346658902, 8.8920475E-7, 2.27638093E-6, 8.8920475E-7,
                         0.00204697265884};

    double inclination_input = 0.90058989402907408; // 51.6 deg in rad
    double m_input = 1.3;                           // kg
    double q0_input[4] = {0.5, 0.5, -0.18301270189221924, 0.6830127018922193};

    // detumble starting conditions (1 - 5 deg per second magnitude)
    double wx_input = 0.05;
    double wy_input = -0.04;
    double wz_input = -0.01;
#endif
};

#endif
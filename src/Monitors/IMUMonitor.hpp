#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "Adafruit_LSM9DS1.h"
#include "ClockManager.cpp"
#include "Control Tasks/TimedControlTask.hpp"
#include "SensorReading.hpp"

class IMUMonitor : public TimedControlTask<void>
{
public:
    IMUMonitor(unsigned int offset);
    void execute();
    Adafruit_LSM9DS1 imu;

    void transition_to_normal();
    void transition_to_abnormal_init();
    void transition_to_retry();

    SensorReading *mag_x_value = new SensorReading(1, mag_min, mag_max);
    SensorReading *mag_y_value = new SensorReading(1, mag_min, mag_max);
    SensorReading *mag_z_value = new SensorReading(1, mag_min, mag_max);

    SensorReading *gyro_x_value = new SensorReading(1, gyro_min, gyro_max);
    SensorReading *gyro_y_value = new SensorReading(1, gyro_min, gyro_max);
    SensorReading *gyro_z_value = new SensorReading(1, gyro_min, gyro_max);

    SensorReading *mag_x_average = new SensorReading(fault_index_type::mag_x, 20, mag_min, mag_max);
    SensorReading *mag_y_average = new SensorReading(fault_index_type::mag_y, 20, mag_min, mag_max);
    SensorReading *mag_z_average = new SensorReading(fault_index_type::mag_z, 20, mag_min, mag_max);

    SensorReading *gyro_x_average = new SensorReading(fault_index_type::gyro_x, 20, gyro_min, gyro_max);
    SensorReading *gyro_y_average = new SensorReading(fault_index_type::gyro_y, 20, gyro_min, gyro_max);
    SensorReading *gyro_z_average = new SensorReading(fault_index_type::gyro_z, 20, gyro_min, gyro_max);

    SensorReading *acc_x_average = new SensorReading(fault_index_type::acc_x, 20, acc_min, acc_max);
    SensorReading *acc_y_average = new SensorReading(fault_index_type::acc_y, 20, acc_min, acc_max);

    void change_mag_max(uint32_t int_mag_max)
    {
        mag_max = int_mag_max / 100;
        mag_x_value = new SensorReading(1, mag_min, mag_max);
        mag_y_value = new SensorReading(1, mag_min, mag_max);
        mag_z_value = new SensorReading(1, mag_min, mag_max);
    }

private:
    void capture_imu_values();

    float mag_min;
    float mag_max;

    float gyro_min;
    float gyro_max;

    float acc_min;
    float acc_max;

    // LJG TODO confirm how many values should be used for average
};

#endif
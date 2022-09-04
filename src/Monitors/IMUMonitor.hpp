#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "Adafruit_LSM9DS1.h"
#include "ClockManager.cpp"
#include "Control Tasks/TimedControlTask.hpp"
#include "Modes/sensor_mode_type.enum"
#include "sfr.hpp"

class IMUMonitor : public TimedControlTask<void>
{
public:
    IMUMonitor(unsigned int offset);
    void execute();
    Adafruit_LSM9DS1 imu;

    void transition_to_normal();
    void transition_to_abnormal_init();
    void transition_to_retry();

    SensorReading *mag_x_value = new SensorReading(1, 0, 0);
    SensorReading *mag_y_value = new SensorReading(1, 0, 0);
    SensorReading *mag_z_value = new SensorReading(1, 0, 0);

    static SensorReading *gyro_x_value; 
    static SensorReading *gyro_y_value; 
    static SensorReading *gyro_z_value; 

    SensorReading *gyro_y_value = new SensorReading(1, 0, 0);
    SensorReading *gyro_z_value = new SensorReading(1, 0, 0);

    SensorReading *mag_x_average = new SensorReading(fault_index_type::mag_x, 20, 0, 0);
    SensorReading *mag_y_average = new SensorReading(fault_index_type::mag_y, 20, 0, 0);
    SensorReading *mag_z_average = new SensorReading(fault_index_type::mag_z, 20, 0, 0);

    SensorReading *gyro_x_average = new SensorReading(fault_index_type::gyro_x, 20, 0, 0);
    SensorReading *gyro_y_average = new SensorReading(fault_index_type::gyro_y, 20, 0, 0);
    SensorReading *gyro_z_average = new SensorReading(fault_index_type::gyro_z, 20, 0, 0);

    SensorReading *acc_x_average = new SensorReading(fault_index_type::acc_x, 20, 0, 0);
    SensorReading *acc_y_average = new SensorReading(fault_index_type::acc_y, 20, 0, 0);

    void change_mag_max(uint32_t int_mag_max)
    {
        float new_max = int_mag_max / 100;

        mag_x_value = new SensorReading(1, mag_x_value->get_min(), new_max);
        mag_y_value = new SensorReading(1, mag_y_value->get_min(), new_max);
        mag_z_value = new SensorReading(1, mag_z_value->get_min(), new_max);
    }

private:
    void capture_imu_values();

    sensor_mode_type mode;

    // LJG TODO confirm how many values should be used for average
};

#endif
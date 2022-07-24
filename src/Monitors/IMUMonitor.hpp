#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

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

private:
    void capture_imu_values();

    // LJG TODO confirm how many values should be used for average

    SensorReading *mag_x_value = new SensorReading(1, sfr::imu::mag_min, sfr::imu::mag_max);
    SensorReading *mag_y_value = new SensorReading(1, sfr::imu::mag_min, sfr::imu::mag_max);
    SensorReading *mag_z_value = new SensorReading(1, sfr::imu::mag_min, sfr::imu::mag_max);

    SensorReading *gyro_x_value = new SensorReading(1, sfr::imu::gyro_min, sfr::imu::gyro_max);
    SensorReading *gyro_y_value = new SensorReading(1, sfr::imu::gyro_min, sfr::imu::gyro_max);
    SensorReading *gyro_z_value = new SensorReading(1, sfr::imu::gyro_min, sfr::imu::gyro_max);

    SensorReading *mag_x_average = new SensorReading(fault_index_type::mag_x, 20, sfr::imu::mag_min, sfr::imu::mag_max);
    SensorReading *mag_y_average = new SensorReading(fault_index_type::mag_y, 20, sfr::imu::mag_min, sfr::imu::mag_max);
    SensorReading *mag_z_average = new SensorReading(fault_index_type::mag_z, 20, sfr::imu::mag_min, sfr::imu::mag_max);

    SensorReading *gyro_x_average = new SensorReading(fault_index_type::gyro_x, 20, sfr::imu::gyro_min, sfr::imu::gyro_max);
    SensorReading *gyro_y_average = new SensorReading(fault_index_type::gyro_y, 20, sfr::imu::gyro_min, sfr::imu::gyro_max);
    SensorReading *gyro_z_average = new SensorReading(fault_index_type::gyro_z, 20, sfr::imu::gyro_min, sfr::imu::gyro_max);

    SensorReading *acc_x_average = new SensorReading(fault_index_type::acc_x, 20, sfr::imu::acc_min, sfr::imu::acc_max);
    SensorReading *acc_y_average = new SensorReading(fault_index_type::acc_y, 20, sfr::imu::acc_min, sfr::imu::acc_max);
};

#endif
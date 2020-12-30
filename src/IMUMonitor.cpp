#include "IMUMonitor.hpp"
#include "constants.hpp"

IMUMonitor::IMUMonitor(): 
    TimedControlTask<void>(registry,"imu_monitor",offset),
    mag_x("imu.mag_x"),
    mag_y("imu.mag_y"),
    mag_z("imu.mag_z"),
    gyro_x("imu.gyro_x"),
    gyro_y("imu.gyro_y"),
    gyro_z("imu.gyro_z") {

    add_internal_field(mag_x);
    add_internal_field(mag_y);
    add_internal_field(mag_z);
    add_internal_field(gyro_x);
    add_internal_field(gyro_y);
    add_internal_field(gyro_z);

    imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
    imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
    imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);

    imu.begin();
}

void IMUMonitor::execute(){
    sensors_event_t accel, mag, gyro, temp;
    imu.getEvent(&accel, &mag, &gyro, &temp);
    mag_x.set(mag.magnetic.x);
    mag_y.set(mag.magnetic.y);
    mag_y.set(mag.magnetic.y);
    gyro_x.set(gyro.gyro.x);
    gyro_y.set(gyro.gyro.y);
    gyro_z.set(gyro.gyro.z);
}

#include "IMUMonitor.hpp"

IMUMonitor::IMUMonitor(){
    imu.begin();
    if (!imu.begin()){
        //sfr::rockblock::fault_report[constants::fault::imu_begin] = 1;
    }
    imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
    imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
    imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
}

void IMUMonitor::execute(){
    sensors_event_t accel, mag, gyro, temp;
    imu.getEvent(&accel, &mag, &gyro, &temp);

    if (mag.acceleration.x > 2 || mag.acceleration.y > 2 || mag.acceleration.z > 2) {
        sfr::imu::acc = 4;
        imu.setupAccel(imu.LSM9DS1_ACCELRANGE_4G);
    }
    if (mag.acceleration.x > 4 || mag.acceleration.y > 4 || mag.acceleration.z > 4) {
        sfr::imu::acc = 8;
        imu.setupAccel(imu.LSM9DS1_ACCELRANGE_8G);
    }
    if (mag.acceleration.x > 8 || mag.acceleration.y > 8 || mag.acceleration.z > 8) {
        sfr::imu::acc = 16;
        imu.setupAccel(imu.LSM9DS1_ACCELRANGE_16G);
    } else {
        sfr::imu::acc = 2;
        imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
    }

    if (mag.magnetic.x > 4 || mag.magnetic.y > 4 || mag.magnetic.z > 4) {
        sfr::imu::mag = 8;
        imu.setupMag(imu.LSM9DS1_MAGGAIN_8GAUSS);
    }
    if (mag.magnetic.x > 8 || mag.magnetic.y > 8 || mag.magnetic.z > 8) {
        sfr::imu::mag = 12;
        imu.setupMag(imu.LSM9DS1_MAGGAIN_12GAUSS);
    }
    if (mag.magnetic.x > 12 || mag.magnetic.y > 12 || mag.magnetic.z > 12) {
        sfr::imu::mag = 16;
        imu.setupMag(imu.LSM9DS1_MAGGAIN_16GAUSS);
    } else {
        sfr::imu::mag = 4;
        imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
    }

    if (mag.gyro.x > 245 || mag.gyro.y > 245 || mag.gyro.z > 245) {
        sfr::imu::gyr = 500;
        imu.setupGyro(imu.LSM9DS1_GYROSCALE_500DPS);
    }
    if (mag.gyro.x > 500 || mag.gyro.y > 500 || mag.gyro.z > 500) {
        sfr::imu::gyr = 2000;
        imu.setupGyro(imu.LSM9DS1_GYROSCALE_2000DPS);
    } else {
        sfr::imu::gyr = 245;
        imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
    }

    imu.getEvent(&accel, &mag, &gyro, &temp);

    sfr::imu::mag_x = mag.magnetic.x;
    sfr::imu::mag_y = mag.magnetic.y;
    sfr::imu::mag_z = mag.magnetic.z;

    sfr::imu::gyro_x = gyro.gyro.x;
    sfr::imu::gyro_y = gyro.gyro.y;
    sfr::imu::gyro_z = gyro.gyro.z;

    sfr::imu::acc_x = accel.acceleration.x;
    sfr::imu::acc_y = accel.acceleration.y;
    sfr::imu::acc_z = accel.acceleration.z;
}

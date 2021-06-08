#include "IMUMonitor.hpp"

IMUMonitor::IMUMonitor(unsigned int offset)
    : TimedControlTask<void>(offset){
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

    imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
    if (accel.acceleration.x > constants::imu::two_g || accel.acceleration.y > constants::imu::two_g || accel.acceleration.z > constants::imu::two_g) {
        imu.setupAccel(imu.LSM9DS1_ACCELRANGE_4G);
    }
    if (accel.acceleration.x > constants::imu::four_g || accel.acceleration.y > constants::imu::four_g || accel.acceleration.z > constants::imu::four_g) {
        imu.setupAccel(imu.LSM9DS1_ACCELRANGE_8G);
    }
    if (accel.acceleration.x > constants::imu::eight_g || accel.acceleration.y > constants::imu::eight_g || accel.acceleration.z > constants::imu::eight_g) {
        imu.setupAccel(imu.LSM9DS1_ACCELRANGE_16G);
    } 

    imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
    if (mag.magnetic.x > 4 || mag.magnetic.y > 4 || mag.magnetic.z > 4) {
        imu.setupMag(imu.LSM9DS1_MAGGAIN_8GAUSS);
    }
    if (mag.magnetic.x > 8 || mag.magnetic.y > 8 || mag.magnetic.z > 8) {
        imu.setupMag(imu.LSM9DS1_MAGGAIN_12GAUSS);
    }
    if (mag.magnetic.x > 12 || mag.magnetic.y > 12 || mag.magnetic.z > 12) {
        imu.setupMag(imu.LSM9DS1_MAGGAIN_16GAUSS);
    } 

    imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
    if (gyro.gyro.x > 245 || gyro.gyro.y > 245 || gyro.gyro.z > 245) {
        imu.setupGyro(imu.LSM9DS1_GYROSCALE_500DPS);
    }
    if (gyro.gyro.x > 500 || gyro.gyro.y > 500 || gyro.gyro.z > 500) {
        imu.setupGyro(imu.LSM9DS1_GYROSCALE_2000DPS);
    } 

    float total_mag_x = 0.0, total_mag_y = 0.0, total_mag_z = 0.0;
    float total_gyro_x = 0.0, total_gyro_y = 0.0, total_gyro_z = 0.0;
    float total_acc_x = 0.0, total_acc_y = 0.0, total_acc_z = 0.0;

    for(size_t i = 0; i < constants::sensor::collect; i++) {
        imu.getEvent(&accel, &mag, &gyro, &temp);
        
        total_mag_x += mag.magnetic.x;
        total_mag_y += mag.magnetic.y;
        total_mag_z += mag.magnetic.z;

        total_gyro_x += gyro.gyro.x;
        total_gyro_y += gyro.gyro.y;
        total_gyro_z += gyro.gyro.z;

        total_acc_x += accel.acceleration.x;
        total_acc_y += accel.acceleration.y;
        total_acc_z += accel.acceleration.z;
    }

    sfr::imu::mag_x = mag.magnetic.x;
    sfr::imu::mag_y = mag.magnetic.y;
    sfr::imu::mag_z = mag.magnetic.z;

    sfr::imu::gyro_x = gyro.gyro.x;
    sfr::imu::gyro_y = gyro.gyro.y;
    sfr::imu::gyro_z = gyro.gyro.z;

    sfr::imu::acc_x = accel.acceleration.x;
    sfr::imu::acc_y = accel.acceleration.y;
    sfr::imu::acc_z = accel.acceleration.z;

    sfr::imu::mag_x_average = total_mag_x / constants::sensor::collect;
    sfr::imu::mag_y_average = total_mag_y / constants::sensor::collect;
    sfr::imu::mag_z_average = total_mag_z / constants::sensor::collect;

    sfr::imu::gyro_x_average = total_gyro_x / constants::sensor::collect;
    sfr::imu::gyro_y_average = total_gyro_y / constants::sensor::collect;
    sfr::imu::gyro_z_average = total_gyro_z / constants::sensor::collect;

    sfr::imu::acc_x_average = total_acc_x / constants::sensor::collect;
    sfr::imu::acc_y_average = total_acc_y / constants::sensor::collect;
    sfr::imu::acc_z_average = total_acc_z / constants::sensor::collect;
}

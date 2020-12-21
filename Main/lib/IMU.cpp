#include "IMU.h"
#include "Constants.h"

IMU::IMU(): lsm(Adafruit_LSM9DS1()){
}

void IMU::setup(){
    lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G); 
    lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
    lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
    lsm.begin();
}

float IMU::getGyroX(){
    updateVals();
    return gyroX * Constants::MODEL_COMPENSATION;
}

float IMU::getGyroY(){
    updateVals();
    return gyroY * Constants::MODEL_COMPENSATION;
}

float IMU::getGyroZ(){
    updateVals();
    return gyroZ * Constants::MODEL_COMPENSATION;
}

float IMU::getMagX(){
    updateVals();
    return magX;
}

float IMU::getMagY(){
    updateVals();
    return magY;
}

float IMU::getMagZ(){
    updateVals();
    return magZ;
}

void IMU::updateVals(){
    lsm.getEvent(&accel, &mag, &gyro, &temp);
    gyroX = gyro.gyro.x;
    gyroY = gyro.gyro.y;
    gyroZ = gyro.gyro.z;
    magX = mag.magnetic.x;
    magY = mag.magnetic.y;
    magZ = mag.magnetic.z;
}
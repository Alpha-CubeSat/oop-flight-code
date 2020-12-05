#ifndef IMU_H
#define IMU_H

#include <Adafruit_LSM9DS1.h>

class IMU{
    public:
    IMU();
    void setup();
    float getGyroX();
    float getGyroY();
    float getGyroZ();
    float getMagX();
    float getMagY();
    float getMagZ();
    void updateVals();

    private:
    Adafruit_LSM9DS1 lsm;
    sensors_event_t accel, mag, gyro, temp;
    float gyroX;
    float gyroY;
    float gyroZ;
    float magX;
    float magY;
    float magZ;


};

#endif
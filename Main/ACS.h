#ifndef ACS_H
#define ACS_H

#include "StarshotACS0.h"
#include <Arduino.h>
#include <TimerOne.h>
#include "Constants.h"
#include "IMU.h"

class ACS{

    public:
    ACS();
    
    void myISR(void);
    void setup();
    void run();
    void setCurrent();
    void ACSwrite(float current,  int out1,  int out2, int PWMpin);

    private:
    StarshotACS0ModelClass rtObj;
    boolean detuble;
    IMU imu;
    float current1;
    float current2;
    float current3;

};

#endif
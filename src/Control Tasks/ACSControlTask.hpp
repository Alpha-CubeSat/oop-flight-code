#ifndef ACS_CONTROL_TASK_HPP_
#define ACS_CONTROL_TASK_HPP_

#include "Modes/acs_mode_type.enum"
#include "Modes/mag_type.enum"
#include "StarshotACS.h"
#include "constants.hpp"

#include "sfr.hpp"

class ACSControlTask
{
public:
    ACSControlTask();
    StarshotACS starshotObj;

    void execute();

private:
    void ACSWrite(int torqorder, float current, int out1, int out2, int PWMpin);
    int current2PWM(float current);

    float mag_x;
    float mag_y;
    float mag_z;

    float gyro_x;
    float gyro_y;
    float gyro_z;

    float num_steps;
    bool first = true;
    float old_Id;
    float old_Kd;
    float old_Kp;
    float old_c;
};

#endif
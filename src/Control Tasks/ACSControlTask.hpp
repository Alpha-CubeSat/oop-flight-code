#ifndef ACS_CONTROL_TASK_HPP_
#define ACS_CONTROL_TASK_HPP_

#include "Modes/acs_mode_type.enum"
#include "Modes/mag_type.enum"
#include "Pins.hpp"
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

    bool first = true;
    bool imu_valid = true;
    
    // PWM buffer, so it sends the magtorqors immely but delay one cycle to the sfr pwm
    int prev_PWMx = 0;
    int prev_PWMy = 0;
    int prev_PWMz = 0;

    float mag_x;
    float mag_y;
    float mag_z;

    float gyro_x;
    float gyro_y;
    float gyro_z;

    float old_Id;
    float old_Kd;
    float old_Kp;
    float old_c;
    float old_target_spin_rate;
};

#endif
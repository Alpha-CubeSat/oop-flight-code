#ifndef ACS_CONTROL_TASK_HPP_
#define ACS_CONTROL_TASK_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "Modes/acs_mode_type.enum"
#include "Modes/mag_type.enum"
#include "Plant.h"
#include "StarshotACS.h"
#include "constants.hpp"
#include "sfr.hpp"

class ACSControlTask : public TimedControlTask<void>
{
public:
    ACSControlTask(unsigned int offset);
    StarshotACS starshotObj;
    Plant plantObj;
    void execute();

private:
    void ACSWrite(int torqorder, float current, int out1, int out2, int PWMpin);
    int current2PWM(float current);
    void IMUOffset(float *mag_x, float *mag_y, float *mag_z, float temp, float voltage, float pwmX, float pwmY, float pwmZ);
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float mag_x;
    float mag_y;
    float mag_z;
    float temp_c;
    float voltage;
    float pwm_x;
    float pwm_y;
    float pwm_z;
    float current_x;
    float current_y;
    float current_z;
    bool imu_valid;
    float num_steps;
};

#endif
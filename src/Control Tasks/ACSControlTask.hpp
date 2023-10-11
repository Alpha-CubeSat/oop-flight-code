#ifndef ACS_CONTROL_TASK_HPP_
#define ACS_CONTROL_TASK_HPP_

#include "Modes/acs_mode_type.enum"
#include "Modes/mag_type.enum"
#include "Plant.h"
#include "StarshotACS.h"
#include "constants.hpp"
#include "sfr.hpp"

class ACSControlTask 
{
public:
    ACSControlTask();
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
    bool first = true;
    float old_Id;
    float old_Kd;
    float old_Kp;
    float old_c;

#ifdef ACS_SIM
    double altitude_input = 400;
    double I_input[9] = {0.00195761450869, -5.836632382E-5, 2.27638093E-6,
                         -5.836632382E-5, 0.00196346658902, 8.8920475E-7, 2.27638093E-6, 8.8920475E-7,
                         0.00204697265884};

    double inclination_input = 0.90058989402907408; // 51.6 deg in rad
    double m_input = 1.3;                           // kg
    double q0_input[4] = {0.5, 0.5, -0.18301270189221924, 0.6830127018922193};
    double wx_input = 0.0;
    double wy_input = 0.0;
    double wz_input = 1.0;
    float last_time = 0;
#endif
};

#endif
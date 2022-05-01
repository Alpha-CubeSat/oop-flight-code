#ifndef ACS_CONTROL_TASK_HPP_
#define ACS_CONTROL_TASK_HPP_

#include "sfr.hpp"

class ACSControlTask : public TimedControlTask<void> {
public:
    ACSControlTask(unsigned int offset);
    void execute();

private:
    void ACSWrite(int torqorder, float current, int out1, int out2, int PWMpin);
    void IMUOffset(float temp, float voltage, float pwm, float pwm2, float pwm3);
    int current2PWM(float current);
};

#endif
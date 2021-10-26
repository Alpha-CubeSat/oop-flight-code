#ifndef ACS_CONTROL_TASK_HPP_
#define ACS_CONTROL_TASK_HPP_

#include "sfr.hpp"

class ACSControlTask : public TimedControlTask<void>
{
public:
    ACSControlTask(unsigned int offset);
    void execute();

private:
    void ACSWrite(int torqorder,  float current,  int out1,  int out2, int PWMpin);
    int current2PWM(float current);
};

#endif
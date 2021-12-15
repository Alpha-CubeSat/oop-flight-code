#ifndef ACS_CONTROL_TASK_HPP_
#define ACS_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "ControlTaskExecute.hpp"

TestMissionManager
class ACSControlTask : public TimedControlTask<void>, public virtual ControlTaskExecute
main
{
public:
    ACSControlTask(unsigned int offset);
    void execute();

private:
TestMissionManager
    void ACSWrite(int torqorder,  float current,  int out1,  int out2, int PWMpin);
    int current2PWM(float current);
main
};

#endif
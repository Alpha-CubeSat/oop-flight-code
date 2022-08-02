#ifndef ACS_CONTROL_TASK_HPP_
#define ACS_CONTROL_TASK_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "sfr.hpp"

class ACSControlTask : public TimedControlTask<void>
{
public:
    ACSControlTask(unsigned int offset);
    void execute();
};

#endif
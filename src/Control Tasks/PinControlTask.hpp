#ifndef PIN_CONTROL_TASK_HPP_
#define PIN_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "ControlTaskExecute.hpp"

class PinControlTask : public virtual ControlTaskExecute
{
public:
    PinControlTask();
    void execute();
};

#endif
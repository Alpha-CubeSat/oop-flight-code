#ifndef TEMPERATURE_CONTROL_TASK_HPP_
#define TEMPERATURE_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include "Control Tasks/TimedControlTask.hpp"


class TemperatureControlTask: public TimedControlTask<void>{

    public:
        TemperatureControlTask(unsigned int offset);
        void execute();
       
};

#endif
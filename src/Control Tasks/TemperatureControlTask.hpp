#ifndef TEMPERATURE_CONTROL_TASK_HPP_
#define TEMPERATURE_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "constants.hpp"

class TemperatureControlTask{

    public:
        TemperatureControlTask();
        void execute();
       
};

#endif
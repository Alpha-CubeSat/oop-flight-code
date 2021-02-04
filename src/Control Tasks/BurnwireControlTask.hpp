#ifndef BURNWIRE_CONTROL_TASK_HPP_
#define BURNWIRE_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <Arduino.h> 

class BurnwireControlTask{
    public:
        BurnwireControlTask();
        void execute();
        int start_burn_time = 0;
};

#endif
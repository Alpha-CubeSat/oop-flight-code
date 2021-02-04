#ifndef BURNWIRE_CONTROL_TASK_HPP_
#define BURNWIRE_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <Arduino.h> 

class BurnwireControlTask{
    public:
        BurnwireControlTask();
        void execute();
        int start_time = 0;
        int attempts = 0;
};

#endif
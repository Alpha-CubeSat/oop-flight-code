#ifndef ACS_CONTROL_TASK_HPP_
#define ACS_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <Arduino.h> 
#include <StarshotACS0.h>

class ACSControlTask{
    public:
        ACSControlTask();
        void execute();
        StarshotACS0ModelClass rtObj;
    private:
        void ACSWrite(int torqorder, float current, int out1, int out2, int PWMpin);
};

#endif
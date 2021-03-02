#ifndef ACS_CONTROL_TASK_HPP_
#define ACS_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "constants.hpp"

class ACSControlTask{
    public:
        ACSControlTask();
        void execute();
    private:
        void ACSWrite(int torqorder, int current, int out1, int out2, int PWMpin);
};

#endif
#ifndef ROCKBLOCK_CONTROL_TASK_HPP_
#define ROCKBLOCK_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <Arduino.h>


class RockblockControlTask{
    public:
        RockblockControlTask();
        void execute();
    private:
        void handleCommand(int opcode, int argument);
        bool checkReady();
};

#endif
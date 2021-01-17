#ifndef ROCKBLOCK_CONTROL_TASK_HPP_
#define ROCKBLOCK_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <IridiumSBD.h>
#include <Arduino.h>


class RockblockControlTask{
    public:
        RockblockControlTask();
        void execute();
        IridiumSBD rockblock;
    private:
        unsigned char receive_buffer[6] = {0};
        size_t receive_size = 6;
        int isFault = 0;
        int quality = 0;
        int waitingMessages = -1;
        void handleCommand(int opcode, int argument);
        bool checkReady();
};

#endif
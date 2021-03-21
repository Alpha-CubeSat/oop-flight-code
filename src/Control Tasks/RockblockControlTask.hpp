#ifndef ROCKBLOCK_CONTROL_TASK_HPP_
#define ROCKBLOCK_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "constants.hpp"

class RockblockControlTask{
    public:
        RockblockControlTask();
        void execute();
    private:
        void handle_command(int opcode, int argument);
        bool check_ready();
        void dispatch_standby();
        void dispatch_send_at();
        void dispatch_await_at();
        void dispatch_send_flow_control();
        void dispatch_await_flow_control();
        void dispatch_send_message_length();
        void dispatch_await_message_length();
        void dispatch_send_message();
        void dispatch_await_message();
        void dispatch_send_response();
        void dispatch_create_buffer();
        void dispatch_process_mo_status();
        void dispatch_process_mt_status();
        void dispatch_read_message();
        void dispatch_process_opcode();
        void dispatch_process_argument();
        void transition_to(rockblock_mode_type new_mode);
        
};

#endif
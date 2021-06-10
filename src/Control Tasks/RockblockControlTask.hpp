#ifndef ROCKBLOCK_CONTROL_TASK_HPP_
#define ROCKBLOCK_CONTROL_TASK_HPP_

#include "sfr.hpp"

class RockblockControlTask : public TimedControlTask<void>{
    public:
        RockblockControlTask(unsigned int offset);
        void execute();
    private:
        void handle_command(int opcode, int argument);

        std::string read_data();
        std::string read_data_no_print();
        void print_data(std::string s);
        size_t indexOf(std::string str, char test);
        size_t indexOf(std::string str, std::string test);
        bool contains(std::string str, char test);
        bool contains(std::string str, std::string test);

        bool check_ready();
        void dispatch_standby();
        void dispatch_send_at();
        void dispatch_await_at();
        void dispatch_send_signal_strength();
        void dispatch_await_signal_strength();
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
        void dispatch_process_command();
        void dispatch_end_transmission();
        void transition_to(rockblock_mode_type new_mode);
        bool valid_command();
        
        
};

#endif
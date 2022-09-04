#ifndef ROCKBLOCK_CONTROL_TASK_HPP_
#define ROCKBLOCK_CONTROL_TASK_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "Modes/rockblock_mode_type.enum"
#include "RockblockCommand.hpp"
#include "RockblockSimulator.hpp"

class RockblockControlTask : public TimedControlTask<void>
{
public:
    RockblockControlTask(unsigned int offset);
    void execute();
private:
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
    void dispatch_send_signal_strength_mo();
    void dispatch_await_signal_strength_mo();
    void dispatch_process_mt_status();
    void dispatch_read_message();
    void dispatch_process_command();
    void dispatch_queue_check();
    void dispatch_send_flush();
    void dispatch_await_flush();
    void dispatch_end_transmission();
    void transition_to(rockblock_mode_type new_mode);
    bool check_valid_command(RawRockblockCommand raw_command);
    void check_timeout();
    #ifndef SIMULATOR
        HardwareSerial serial = Serial1;
    #else
        RockblockSimulator serial;
    #endif
};

#endif
#ifndef ROCKBLOCK_CONTROL_TASK_HPP_
#define ROCKBLOCK_CONTROL_TASK_HPP_

#include "Modes/rockblock_mode_type.enum"
#include "Pins.hpp"
#include "RockblockCommand.hpp"
#include "RockblockSimulator.hpp"
#include "sfr.hpp"

class RockblockControlTask
{
public:
    RockblockControlTask();
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
    void dispatch_await_message_ok();
    void dispatch_send_response();
    void dispatch_create_buffer();
    void dispatch_process_mo_status();
    void dispatch_send_signal_strength_mo();
    void dispatch_await_signal_strength_mo();
    void dispatch_process_mt_status();
    void dispatch_read_message_ok();
    void dispatch_read_message();
    void dispatch_process_command();
    void dispatch_queue_check();
    void dispatch_send_flush();
    void dispatch_await_flush();
    void dispatch_end_transmission();
    bool get_OK();
    void get_valid_signal(rockblock_mode_type good_signal, rockblock_mode_type bad_signal);
    void transition_to(rockblock_mode_type new_mode);
    RockblockCommand *commandFactory(RawRockblockCommand raw);
    uint32_t conseq_reads = 0;
    uint32_t start_time_check_signal = 0;
    uint32_t same_mode = 0;
    uint8_t look_ahead1 = -1;
    uint8_t look_ahead2 = -1;
};

#endif
#ifndef COMMAND_MONITOR_HPP_
#define COMMAND_MONITOR_HPP_

#include "sfr.hpp"

class CommandMonitor : public TimedControlTask<void>{
    public:
        CommandMonitor(unsigned int offset);
        void execute();
        void dispatch_change_mission_mode();
        void dispatch_change_true_false(bool value);
        void dispatch_change_temperature_mode();
        void dispatch_change_acs_mode();
        void dispatch_change_fault_mode();
        void dispatch_request_image_fragment();
        void dispatch_change_rockblock_downlink_period();
        int get_decimal_opcode(uint8_t hex_opcode_byte_one, uint8_t hex_opcode_byte_two);
        int get_decimal_arg(uint8_t hex_arg_byte_one, uint8_t hex_arg_byte_two, uint8_t hex_arg_byte_three, uint8_t hex_arg_byte_four);
};

#endif
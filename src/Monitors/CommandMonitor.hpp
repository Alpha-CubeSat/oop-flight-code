#ifndef COMMAND_MONITOR_HPP_
#define COMMAND_MONITOR_HPP_

#include "sfr.hpp"

class CommandMonitor : public TimedControlTask<void>
{
public:
    CommandMonitor(unsigned int offset);
    void execute();
    // bool execute_sfr_data_override(RockblockCommand &command);
    // void dispatch_change_mission_mode();
    // void dispatch_change_true_false(bool &value, uint32_t to_value);
    // void dispatch_change_temperature_mode();
    // void dispatch_change_acs_mode();
    // void dispatch_change_fault_mode(uint32_t f_arg_1);
    // void dispatch_request_image_fragment(uint32_t f_arg_1, uint32_t f_arg_2);
    // void dispatch_change_rockblock_downlink_period(uint32_t f_arg_1);
    // void dispatch_change_burnwire_time(uint32_t f_arg_1);
    // void dispatch_change_burnwire_timeout(uint32_t f_arg_1);
    // void dispatch_change_simplified_acs(uint32_t f_arg_1);
    // uint16_t get_decimal_opcode(const uint8_t *hex_opcode_bytes);
    // uint32_t get_decimal_arg(const uint8_t *hex_arg_bytes);
};

#endif
#ifndef COMMAND_MONITOR_HPP_
#define COMMAND_MONITOR_HPP_

#include "sfr.hpp"

class CommandMonitor : public TimedControlTask<void> {
public:
    CommandMonitor(unsigned int offset);
    void execute();
    void dispatch_change_mission_mode();
    void dispatch_change_true_false(bool& value);
    void dispatch_change_temperature_mode();
    void dispatch_change_acs_mode();
    void dispatch_change_fault_mode();
    void dispatch_request_image_fragment();
    void dispatch_change_rockblock_downlink_period();
    void dispatch_change_burnwire_time();
    void dispatch_change_burnwire_timeout();
    void dispatch_change_simplified_acs();
    uint16_t get_decimal_opcode(const uint8_t* hex_opcode_bytes);
    uint32_t get_decimal_arg(const uint8_t* hex_arg_bytes);
};

#endif
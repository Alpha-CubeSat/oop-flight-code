#ifndef BURNWIRE_CONTROL_TASK_HPP_
#define BURNWIRE_CONTROL_TASK_HPP_

#include "Modes/burnwire_mode_type.enum"
#include "Pins.hpp"
#include "sfr.hpp"

class BurnwireControlTask
{
public:
    BurnwireControlTask();
    void execute();
    void dispatch_burn();
    void transition_to_standby();
    void transition_to_delay();
};

#endif
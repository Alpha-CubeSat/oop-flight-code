#ifndef BURNWIRE_CONTROL_TASK_HPP_
#define BURNWIRE_CONTROL_TASK_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "Modes/burnwire_mode_type.enum"
#include "Pins.hpp"
#include "sfr.hpp"

class BurnwireControlTask : public TimedControlTask<void>
{
public:
    BurnwireControlTask(unsigned int offset);
    void execute();
    void dispatch_burn();
    void transition_to_standby();
    void transition_to_delay();
};

#endif
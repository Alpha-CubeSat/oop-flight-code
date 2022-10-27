#ifndef BURNWIRE_CONTROL_TASK_HPP_
#define BURNWIRE_CONTROL_TASK_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "Pins.hpp"
#include "sfr.hpp"
#include "Modes/burnwire_mode_type.enum"

class BurnwireControlTask : public TimedControlTask<void>
{
public:
    BurnwireControlTask(unsigned int offset);
    void execute();
    void mandatory_dispatch_burn();
    void regular_dispatch_burn();

    burnwire_mode_type mode;
};

#endif
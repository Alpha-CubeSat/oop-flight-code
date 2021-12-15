#ifndef BURNWIRE_CONTROL_TASK_HPP_
#define BURNWIRE_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include "ControlTaskExecute.hpp"

TestMissionManager
class BurnwireControlTask : public TimedControlTask<void>, public virtual ControlTaskExecute
main
{
public:
    BurnwireControlTask(unsigned int offset);
    void execute();
    void dispatch_burn();
    static void transition_to_standby();
};

#endif
#ifndef BURNWIRE_CONTROL_TASK_HPP_
#define BURNWIRE_CONTROL_TASK_HPP_

#include "sfr.hpp"

class BurnwireControlTask : public TimedControlTask<void>{
    public:
        BurnwireControlTask(unsigned int offset);
        void execute();
        void dispatch_burn();
        static void transition_to_standby();
};

#endif
#ifndef PHOTORESISTOR_MONITOR_HPP_
#define PHOTORESISTOR_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include "Control Tasks/TimedControlTask.hpp"

class PhotoresistorMonitor : public TimedControlTask<void>{
    public:
        PhotoresistorMonitor(unsigned int offset);
        void execute();
};

#endif
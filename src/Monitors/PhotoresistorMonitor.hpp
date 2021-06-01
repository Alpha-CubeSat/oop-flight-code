#ifndef PHOTORESISTOR_MONITOR_HPP_
#define PHOTORESISTOR_MONITOR_HPP_

#include "sfr.hpp"

class PhotoresistorMonitor : public TimedControlTask<void>{
    public:
        PhotoresistorMonitor(unsigned int offset);
        void execute();
};

#endif
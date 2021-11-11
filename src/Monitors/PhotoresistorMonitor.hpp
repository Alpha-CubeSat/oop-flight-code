#ifndef PHOTORESISTOR_MONITOR_HPP_
#define PHOTORESISTOR_MONITOR_HPP_

#include "sfr.hpp"
#include "MonitorsExecute.hpp"

class PhotoresistorMonitor : public TimedControlTask<void>, public virtual MonitorsExecute
{
public:
    PhotoresistorMonitor(unsigned int offset);
    void execute();
};

#endif
#ifndef PHOTORESISTOR_MONITOR_HPP_
#define PHOTORESISTOR_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "constants.hpp"
#include "sfr.hpp"
#include <deque>

class PhotoresistorMonitor : public TimedControlTask<void>
{
public:
    PhotoresistorMonitor(unsigned int offset);
    void execute();

private:
    bool initialized = false;
    bool avg_buffer_valid;
    bool deploy_buffer_valid;
    float val;
};

#endif
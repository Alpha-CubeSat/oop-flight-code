#ifndef PHOTORESISTOR_MONITOR_HPP_
#define PHOTORESISTOR_MONITOR_HPP_

#include "constants.hpp"
#include "sfr.hpp"
#include <deque>

class PhotoresistorMonitor 
{
public:
    PhotoresistorMonitor();
    void execute();

private:
    bool initialized = false;
    bool avg_buffer_valid;
    bool deploy_buffer_valid;
    float val;
};

#endif
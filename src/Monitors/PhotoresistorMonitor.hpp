#ifndef PHOTORESISTOR_MONITOR_HPP_
#define PHOTORESISTOR_MONITOR_HPP_

#include "sfr.hpp"

class PhotoresistorMonitor : public TimedControlTask<void>
{
public:
    PhotoresistorMonitor(unsigned int offset);
    void execute();

private:
    void capture_photoresistor_value();
    void check_door_closed();
};

#endif
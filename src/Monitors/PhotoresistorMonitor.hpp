#ifndef PHOTORESISTOR_MONITOR_HPP_
#define PHOTORESISTOR_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include <deque>
#include "constants.hpp"
#include "sfr.hpp"

class PhotoresistorMonitor : public TimedControlTask<void>
{
public:
    PhotoresistorMonitor(unsigned int offset);
    void execute();
    bool check_repeated_values(std::deque<int> buffer);
    bool check_invalid_reading(std::deque<int> buffer);

private:
    void capture_photoresistor_value();
    
};

#endif
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
    bool check_repeated_values(std::deque<int> buffer);
    bool check_invalid_reading(std::deque<int> buffer);

private:
    void capture_photoresistor_value();
    bool initialized = false;
};

#endif
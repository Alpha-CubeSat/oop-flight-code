#ifndef PHOTORESISTOR_MONITOR_HPP_
#define PHOTORESISTOR_MONITOR_HPP_

#include "SensorReading.hpp"
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

    // LJG TODO constants set light val min and max

    SensorReading *light_val_average = new SensorReading(fault_index_type::light_val, 20, 0, 1000);
};

#endif
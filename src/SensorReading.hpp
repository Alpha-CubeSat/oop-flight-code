#ifndef _SENSORREADING_HPP_
#define _SENSORREADING_HPP_

#include "sfr.hpp"
#include <map>

class SensorReading
{
private:
    fault_index_type type; // Type of reading
    float value;           // Sensor reading (set to the averaged value in SFR)
    bool fault_status;     // Fault flag: true means invalid reading and false means valid reading

public:
    SensorReading(fault_index_type t, float x, bool fault); // Constructor
    float get_value();                                      // Get the sensor reading
    bool is_valid();                                        // Get the fault status
    void set_value(float value);                            // Set the value of a SensorReading object
    void set_valid();                                       // Set the fault flag of a SensorReading object to 1
    void set_invalid();                                     // Set the fault flag of a SensorReading object to 0
};

#endif
#ifndef SENSOR_READING_HPP_
#define SENSOR_READING_HPP_

#include "Modes/fault_index_type.enum"
#include "sfr.hpp"

class SensorReading
{
private:
    fault_index_type type;
    float value;       // Sensor reading (set to the averaged value in SFR)
    bool fault_status; // Fault flag: true means valid reading and false means invalid reading

public:
    SensorReading(fault_index_type t, float x, bool fault); // Constructor
    float get_value();                                      // Get the sensor reading
    bool is_valid();                                        // Get the fault status
    void set_value(float value);                            // Set the value of a SensorReading object
    void set_valid();                                       // Set the fault flag of a SensorReading object to 1
    void set_invalid();                                     // Set the fault flag of a SensorReading object to 0
};

#endif
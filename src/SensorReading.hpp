#ifndef _SENSORREADING_HPP_
#define _SENSORREADING_HPP_

#include "Arduino.h"  // allows Serial.println() calls
#include "Faults.hpp" // Cannot include sfr.hpp in this file without creating circular dependency since SensorReading objects are declared in sfr.hpp
#include "Modes/fault_index_type.enum"
#include "constants.hpp"
#include <deque>
#include <map>
#include <numeric>

class SensorReading
{
private:
    fault_index_type type;    // type of reading
    uint8_t buffer_size;      // amount of values that should be averaged
    float max;                // max valid value
    float min;                // min valid value
    std::deque<float> buffer; // buffer used to store the raw values
    bool valid = false;       // if SensorReading is valid
    bool repeated_values(std::deque<float> *buffer, float val);

public:
    SensorReading(fault_index_type type, uint8_t buffer_size, float max, float min); // constructor
    SensorReading(uint8_t buffer_size, float max, float min);                        // constructor
    bool get_value(float *value_location);                                           // get SensorReading averaged value
    void set_value(float x);                                                         // set SensorReading value
    void set_invalid();
    void set_valid();
    float get_min();
    float get_max();
    bool is_valid();
};

#endif
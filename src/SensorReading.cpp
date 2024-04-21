#include "SensorReading.hpp"

SensorReading::SensorReading(Fault *fault, uint16_t buffer_size, float min, float max)
{
    this->fault = fault;
    this->buffer_size = buffer_size;
    this->max = max;
    this->min = min;
} // constructor

bool SensorReading::get_value(float *value_location)
{
    // enough values have been accumulated to get the average
    if (buffer.size() == buffer_size && this->is_valid()) {
        // get average value
        float average = (std::accumulate(buffer.begin(), buffer.end(), 0.0)) / buffer_size;
        *value_location = average;
        return true;
    } else {
        return false;
    }
}

void SensorReading::set_value(float x)
{
    // check if value is within expected range
    if (x <= max && x >= min && !repeated_values(x)) {
        set_valid();
        buffer.push_front(x);
        // check if buffer is full
        if (buffer.size() > buffer_size) {
            // remove oldest value from buffer
            buffer.pop_back();
        }
    } else {
        set_invalid();
    }

    if (buffer.size() != buffer_size) {
        valid = false;
        fault->signal();
    }

} // mutator for buffer

void SensorReading::set_valid()
{
    valid = true;
    fault->release();
}

void SensorReading::set_invalid()
{
    valid = false;
    if (!fault->get_suppressed()) {
        buffer.clear();
    }
    fault->signal();
}

bool SensorReading::repeated_values(float val)
{
    if (buffer.empty() || buffer.size() == 1) {
        return false;
    }

    for (size_t i = 0; i < buffer.size(); i++) {
        if (buffer.at(i) != val) {
            return false;
        }
    }

    return true;
}

bool SensorReading::is_valid()
{
    return !(fault->get_base());
}

float SensorReading::get_min()
{
    return min;
}

float SensorReading::get_max()
{
    return max;
}
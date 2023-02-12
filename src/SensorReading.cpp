#include "SensorReading.hpp"

SensorReading::SensorReading(Fault *type, uint8_t buffer_size, float min, float max)
{
    this->type = type;
    this->buffer_size = buffer_size;
    this->max = max;
    this->min = min;
    buffer.clear();
} // constructor

SensorReading::SensorReading(uint8_t buffer_size, float min, float max)
{
    this->type = NULL;
    this->buffer_size = buffer_size;
    this->max = max;
    this->min = min;
    buffer.clear();
} // constructor

bool SensorReading::get_value(float *value_location)
{
    // enough values have been accumulated to get the average
    if (buffer.size() == buffer_size && valid) {
        // get average value
        float average = (std::accumulate(buffer.begin(), buffer.end(), 0.0)) / buffer_size;
        *value_location = average;
        return 1;
    } else {
        return 0;
    }
}

void SensorReading::set_value(float x)
{
    // check if value is within expected range
    if (x <= max && x >= min) {
        if (valid && !repeated_values(&buffer, x)) {
            // check if buffer is full
            if (buffer.size() > buffer_size) {
                // remove oldest value from buffer
                buffer.pop_back();
            }
        }
    } else {
        set_invalid();
    }

} // mutator for buffer

void SensorReading::set_valid()
// set_valid changes the signal to valid?
{
    valid = true;
    // what does this check against?
    if (type != NULL) {
        type->release();
    }
}

void SensorReading::set_invalid()
{
    valid = false;
    if (type != NULL) {
        type->signal();
    }
}

bool SensorReading::repeated_values(std::deque<float> *buffer, float val)
{
    if (buffer->empty() || buffer->size() == 1) {
        buffer->push_front(val);
        return false;
    }

    int min_loop_max;

    if (buffer->size() > constants::sensor::repeats) {
        min_loop_max = constants::sensor::repeats;
    } else {
        min_loop_max = buffer->size();
    }

    for (int i = 0; i < min_loop_max; i++) {
        if (buffer->at(i) != val) {
            buffer->push_front(val);
            return false;
        }
    }

    return true;
}

float SensorReading::get_max()
{
    return max;
}

float SensorReading::get_min()
{
    return min;
}

bool SensorReading::is_valid()
{
    if (type != NULL) {
        return !(type->get_base());
    }
    return true;
}
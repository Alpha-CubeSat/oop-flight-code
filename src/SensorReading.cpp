#include "SensorReading.hpp"

std::map<fault_index_type, int> map_to_reg = {
    {fault_index_type::mag_x, 1},
    {fault_index_type::mag_y, 1},
    {fault_index_type::mag_z, 1},
    {fault_index_type::gyro_x, 1},
    {fault_index_type::gyro_y, 1},
    {fault_index_type::gyro_z, 1},
    {fault_index_type::acc_x, 1},
    {fault_index_type::acc_y, 1},

    {fault_index_type::temp_c, 2},
    {fault_index_type::solar_current, 2},
    {fault_index_type::voltage, 2},

    {fault_index_type::burn_wire, 3},
    {fault_index_type::sd_card, 3},
    {fault_index_type::camera_on_failed, 3},
}; // map from a fault type to the fault register

std::map<fault_index_type, uint8_t> map_to_mask = {
    {fault_index_type::mag_x, constants::fault::mag_x},
    {fault_index_type::mag_y, constants::fault::mag_y},
    {fault_index_type::mag_z, constants::fault::mag_z},
    {fault_index_type::gyro_x, constants::fault::gyro_x},
    {fault_index_type::gyro_y, constants::fault::gyro_y},
    {fault_index_type::gyro_z, constants::fault::gyro_z},
    {fault_index_type::acc_x, constants::fault::acc_x},
    {fault_index_type::acc_y, constants::fault::acc_y},

    {fault_index_type::temp_c, constants::fault::temp_c},
    {fault_index_type::solar_current, constants::fault::solar_current},
    {fault_index_type::voltage, constants::fault::voltage},

    {fault_index_type::burn_wire, constants::fault::burn_wire},
    {fault_index_type::sd_card, constants::fault::sd_card},
    {fault_index_type::camera_on_failed, constants::fault::camera_on_failed},
}; // map from a fault type to the mask

SensorReading::SensorReading(fault_index_type type, uint8_t buffer_size, float max, float min)
{
    type = type;
    buffer_size = buffer_size;
    max = max;
    min = min;
    buffer.clear();
} // constructor

SensorReading::SensorReading(uint8_t buffer_size, float max, float min)
{
    type = NULL;
    buffer_size = buffer_size;
    max = max;
    min = min;
    buffer.clear();
} // constructor

boolean SensorReading::get_value(float *value_location)
{
    // enough values have been accumulated to get the average
    if (buffer.size() == buffer_size && valid) {
        // get average value
        float average = (std::accumulate(buffer.begin(), buffer.end(), 0.0)) / buffer_size;
        value_location = &average;
        return 1;
    } else {
        return -1;
    }
}

void SensorReading::set_value(float x)
{
    // LJG TODO might need to add whether we want to check for repeated values in the constructor
    // check if value is within expected range
    if (x <= max && x >= min && !repeated_values(buffer)) {
        if (valid) {
            // add value to buffer
            buffer.push_front(x);

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
{
    valid = true;

    if (type != NULL)
        // clear fault bit
        if (map_to_reg[this->type] == 1) {
            faults::fault_1 &= ~map_to_mask[this->type];
        } else if (map_to_reg[this->type] == 2) {
            faults::fault_2 &= ~map_to_mask[this->type];
        } else {
            faults::fault_3 &= ~map_to_mask[this->type];
        } // clear the flag in the corresponding fault register
}

void SensorReading::set_invalid()
{
    valid = false;

    // clear buffer
    buffer.clear();

    if (type != NULL) {
        // set fault bit
        if (map_to_reg[this->type] == 1) {
            faults::fault_1 |= map_to_mask[this->type];
        } else if (map_to_reg[this->type] == 2) {
            faults::fault_2 |= map_to_mask[this->type];
        } else {
            faults::fault_3 |= map_to_mask[this->type];
        } // set the flag in the corresponding fault register
    }
}

bool repeated_values(std::deque<float> buffer)
{
    if (buffer.empty() || buffer.size() == 1) {
        return false;
    }

    int first_val = buffer[0];
    for (float val : buffer) {
        if (first_val != val) {
            return false;
        }
    }

    return true;
}
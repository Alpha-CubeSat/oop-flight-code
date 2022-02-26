#include "SensorReading.hpp"
#include "Modes/fault_index_type.enum"
#include <map>

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

SensorReading::SensorReading(fault_index_type t, float x, bool fault)
{
    type = t;
    value = x;
    fault_status = fault;
} // constructor

float SensorReading::get_value()
{
    return value;
} // accesser for valoue

bool SensorReading::is_valid()
{
    return fault_status;
} // accesser for fault

void SensorReading::set_value(float x)
{
    this->value = x;
} // mutator for value

void SensorReading::set_valid()
{
    this->fault_status = false; // set the fault status to false (no fault)
    if (map_to_reg[this->type] == 1) {
        sfr::fault::fault_1 &= ~map_to_mask[this->type];
    } else if (map_to_reg[this->type] == 1) {
        sfr::fault::fault_2 &= ~map_to_mask[this->type];
    } else {
        sfr::fault::fault_3 &= ~map_to_mask[this->type];
    } // clear the flag in the corresponding fault register
} // mutator for fault status

void SensorReading::set_invalid()
{
    this->fault_status = true; // set the fault status to true
    if (map_to_reg[this->type] == 1) {
        sfr::fault::fault_1 |= map_to_mask[this->type];
    } else if (map_to_reg[this->type] == 1) {
        sfr::fault::fault_2 |= map_to_mask[this->type];
    } else {
        sfr::fault::fault_3 |= map_to_mask[this->type];
    } // set the flag in the corresponding fault register
} // mutator for fault status
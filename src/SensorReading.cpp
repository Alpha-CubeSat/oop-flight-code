#include "SensorReading.hpp"

SensorReading::SensorReading(float x, bool fault)
{
    value = x;
    fault_status = fault;
}

float SensorReading::get_value()
{
    return value;
}

bool SensorReading::is_valid()
{
    return fault_status;
}

void SensorReading::set_value(float x)
{
    this->value = x;
}

void SensorReading::set_valid()
{
    this->fault_status = false;
}

void SensorReading::set_invalid()
{
    this->fault_status = true;
}
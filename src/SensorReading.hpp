#ifndef _SENSORREADING_HPP_
#define _SENSORREADING_HPP_

#include "Arduino.h"  // allows Serial.println() calls
#include "Faults.hpp" // Cannot include sfr.hpp in this file without creating circular dependency since SensorReading objects are declared in sfr.hpp
#include "constants.hpp"
#include <deque>
#include <map>
#include <numeric>

/**
 * @brief Represents the readings coming from a sensor
 *
 */
class SensorReading
{
private:
    /**
     * @brief Pointer to the fault
     *
     */
    Fault *fault;

    /**
     * @brief Size of the buffer, the amount of values that should be averaged
     *
     */
    uint16_t buffer_size;

    /**
     * @brief Max valid value
     *
     */
    float max;

    /**
     * @brief Min valid value
     *
     */
    float min;

    /**
     * @brief Buffer used to store the raw values
     *
     */
    std::deque<float> buffer;

    /**
     * @brief If SensorReading is valid
     *
     */
    bool valid = false;

    bool repeated_values(std::deque<float> *buffer, float val);

public:
    /**
     * @brief Construct a new Sensor Reading object with an associated fault
     *
     * @param fault Pointer to the fault associated with the sensor
     * @param buffer_size Size of the buffer, the amount of values that should be averaged
     * @param min Minimum valid value
     * @param max Maximum valid value
     */
    SensorReading(Fault *fault, uint16_t buffer_size, float min, float max);

    /**
     * @brief Get sensor reading average value
     *
     * @param value_location Where to store the average value
     * @return true when the buffer is full and the sensor is valid,
     * @return false otherwise
     */
    bool get_value(float *value_location);

    /**
     * @brief Set the sensor reading value
     *
     * @param x The value to set the sensor reading to
     */
    void set_value(float x);

    /**
     * @brief Set the sensor to invalid
     *
     */
    void set_invalid();

    /**
     * @brief Set the sensor to valid
     *
     */
    void set_valid();

    /**
     * @brief Get the minimum valid value of the sensor
     *
     * @return float The minimum valid value
     */
    float get_min();

    /**
     * @brief Get the maximum valid value of the sensor
     *
     * @return float The maximum valid value
     */
    float get_max();

    /**
     * @brief Whether the sensor readings are valid
     *
     * @return true when the sensor readings are valid,
     * @return false otherwise
     */
    bool is_valid();
};

#endif
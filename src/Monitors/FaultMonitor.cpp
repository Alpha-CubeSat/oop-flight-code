#include "FaultMonitor.hpp"

FaultMonitor::FaultMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void FaultMonitor::execute()
{

    if (sfr::fault::mode == fault_mode_type::active) {
        // FAULT_1
        if ((sfr::imu::mag_x > constants::imu::max_mag_x || sfr::imu::mag_x < constants::imu::min_mag_x) && sfr::imu::mag_x_average->is_valid()) {
            sfr::imu::mag_x_average->set_invalid();
        }
        if ((sfr::imu::mag_y > constants::imu::max_mag_y || sfr::imu::mag_y < constants::imu::min_mag_y) && sfr::imu::mag_y_average->is_valid()) {
            sfr::imu::mag_y_average->set_invalid();
        }
        if ((sfr::imu::mag_z > constants::imu::max_mag_z || sfr::imu::mag_z < constants::imu::min_mag_z) && sfr::imu::mag_z_average->is_valid()) {
            sfr::imu::mag_z_average->set_invalid();
        }
        if ((sfr::imu::gyro_x > constants::imu::max_gyro_x || sfr::imu::gyro_x < constants::imu::min_gyro_x) && sfr::imu::gyro_x_average->is_valid()) {
            sfr::imu::gyro_x_average->set_invalid();
        }
        if ((sfr::imu::gyro_y > constants::imu::max_gyro_y || sfr::imu::gyro_y < constants::imu::min_gyro_y) && sfr::imu::gyro_y_average->is_valid()) {
            sfr::imu::gyro_x_average->set_invalid();
        }
        if ((sfr::imu::gyro_z > constants::imu::max_gyro_z || sfr::imu::gyro_z < constants::imu::min_gyro_z) && sfr::imu::gyro_z_average->is_valid()) {
            sfr::imu::gyro_x_average->set_invalid();
        }

        // FAULT_2
        if ((sfr::temperature::temp_c > constants::temperature::max_temp_c || sfr::temperature::temp_c < constants::temperature::min_temp_c) && sfr::temperature::temp_c_average->is_valid()) {
            sfr::temperature::temp_c_average->set_invalid();
        }
        if ((sfr::current::solar_current > constants::current::max_solar_current || sfr::current::solar_current < constants::current::min_solar_current) && sfr::current::solar_current_average->is_valid()) {
            sfr::current::solar_current_average->set_invalid();
        }
        if ((sfr::battery::voltage > constants::battery::max_voltage || sfr::battery::voltage < constants::battery::min_voltage) && sfr::battery::voltage_average->is_valid()) {
            sfr::battery::voltage_average->set_invalid();
        }

        // FAULT_3
        if ((sfr::photoresistor::val > constants::photoresistor::max_val || sfr::photoresistor::val < constants::photoresistor::min_val) && sfr::photoresistor::light_val_average->is_valid()) {
            sfr::photoresistor::light_val_average->set_invalid();
        }
    }
}

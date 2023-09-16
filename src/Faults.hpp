#ifndef _FAULTS_HPP_
#define _FAULTS_HPP_

// NOTE: This namespace needs to be separated from the SFR because including sfr.hpp in SensorReading creates a circular dependency
#include "Fault.hpp"
namespace fault_groups {
    namespace imu_faults {
        extern Fault *mag_x_value;
        extern Fault *mag_x_average;
        extern Fault *mag_y_value;
        extern Fault *mag_y_average;
        extern Fault *mag_z_value;
        extern Fault *mag_z_average;
        extern Fault *gyro_x_value;
        extern Fault *gyro_x_average;
        extern Fault *gyro_y_value;
        extern Fault *gyro_y_average;
        extern Fault *gyro_z_value;
        extern Fault *gyro_z_average;
    } // namespace imu_faults
    namespace power_faults {
        extern Fault *temp_c_value;
        extern Fault *temp_c_average;
        extern Fault *solar_current_average;
        extern Fault *voltage_value;
        extern Fault *voltage_average;
    } // namespace power_faults
    namespace hardware_faults {
        extern Fault *light_val;
        extern Fault *button;
    } // namespace hardware_faults

} // namespace fault_groups

#endif
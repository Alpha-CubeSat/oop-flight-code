#ifndef _FAULTS_HPP_
#define _FAULTS_HPP_

// NOTE: This namespace needs to be separated from the SFR because including sfr.hpp in SensorReading creates a circular dependency
#include "Fault.hpp"
namespace fault_groups {
    namespace imu_faults {
        extern Fault *mag_x;
        extern Fault *mag_y;
        extern Fault *mag_z;
        extern Fault *gyro_x;
        extern Fault *gyro_y;
        extern Fault *gyro_z;
        extern Fault *acc_x;
        extern Fault *acc_y;
    } // namespace imu_faults
    namespace power_faults {
        extern Fault *temp_c;
        extern Fault *solar_current;
        extern Fault *voltage;
    } // namespace power_faults
    namespace hardware_faults {
        extern Fault *burn_wire;        // not in use
        extern Fault *sd_card;          // not in use
        extern Fault *camera_on_failed; // not in use
        extern Fault *light_val;
        extern Fault *button;
    } // namespace hardware_faults

} // namespace fault_groups

#endif
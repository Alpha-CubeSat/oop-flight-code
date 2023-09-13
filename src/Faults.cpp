#include "Faults.hpp"

namespace fault_groups {
    namespace imu_faults {
        Fault *mag_x = new Fault(0x6000);
        Fault *mag_y = new Fault(0x6001);
        Fault *mag_z = new Fault(0x6002);
        Fault *gyro_x = new Fault(0x6003);
        Fault *gyro_y = new Fault(0x6004);
        Fault *gyro_z = new Fault(0x6005);
        Fault *acc_x = new Fault(0x6006);
        Fault *acc_y = new Fault(0x6007);
    } // namespace imu_faults
    namespace power_faults {
        Fault *temp_c = new Fault(0x6010);
        Fault *solar_current = new Fault(0x6011);
        Fault *voltage = new Fault(0x6012);
    } // namespace power_faults
    namespace hardware_faults {
        Fault *burn_wire = new Fault(0x6020);
        Fault *sd_card = new Fault(0x6021);
        Fault *camera_on_failed = new Fault(0x6022);
        Fault *light_val = new Fault(0x6023);
        Fault *button = new Fault(0x6024);
    } // namespace hardware_faults
} // namespace fault_groups

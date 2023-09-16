#include "Faults.hpp"

namespace fault_groups {
    namespace imu_faults {
        Fault *mag_x_value = new Fault(0x6000);
        Fault *mag_x_average = new Fault(0x6001);
        Fault *mag_y_value = new Fault(0x6002);
        Fault *mag_y_average = new Fault(0x6003);
        Fault *mag_z_value = new Fault(0x6004);
        Fault *mag_z_average = new Fault(0x6005);
        Fault *gyro_x_value = new Fault(0x6006);
        Fault *gyro_x_average = new Fault(0x6007);
        Fault *gyro_y_value = new Fault(0x6008);
        Fault *gyro_y_average = new Fault(0x6009);
        Fault *gyro_z_value = new Fault(0x6010);
        Fault *gyro_z_average = new Fault(0x6011);
        Fault *acc_x_average = new Fault(0x6012);
        Fault *acc_y_average = new Fault(0x6013);
    } // namespace imu_faults
    namespace power_faults {
        Fault *temp_c_value = new Fault(0x6020);
        Fault *temp_c_average = new Fault(0x6021);
        Fault *solar_current_average = new Fault(0x6022);
        Fault *voltage_value = new Fault(0x6023);
        Fault *voltage_average = new Fault(0x6024);
    } // namespace power_faults
    namespace hardware_faults {
        Fault *light_val = new Fault(0x6030);
        Fault *button = new Fault(0x6031);
    } // namespace hardware_faults
} // namespace fault_groups

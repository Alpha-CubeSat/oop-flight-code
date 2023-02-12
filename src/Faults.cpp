#include "Faults.hpp"

namespace fault_groups {
    namespace imu_faults {
        Fault* mag_x = new Fault();
        Fault* mag_y = new Fault();
        Fault* mag_z = new Fault();
        Fault* gyro_x = new Fault();
        Fault* gyro_y = new Fault();
        Fault* gyro_z = new Fault();
        Fault* acc_x = new Fault();
        Fault* acc_y = new Fault();
    }
    namespace power_faults {
        Fault* temp_c = new Fault();
        Fault* solar_current = new Fault();
        Fault* voltage = new Fault();
    }
    namespace hardware_faults{
        Fault* burn_wire = new Fault();
        Fault* sd_card = mew Fault();
        Fault* camera_on_failed = new Fault();
        Fault* light_val = new Fault();
        Fault* button = new Fault();
    
    
    
} // namespace faults

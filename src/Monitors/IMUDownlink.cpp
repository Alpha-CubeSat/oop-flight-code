#include "IMUDownlink.hpp"

IMUDownlink::IMUDownlink()
{
}

void IMUDownlink::execute()
{
    float gyro_x;
    float gyro_y;
    float gyro_z;
    static bool prev_deployed = false;
    if (sfr::imu::sample_gyro) {

        // Store the time the door has opened
        if (prev_deployed != sfr::mission::deployed) {
            sfr::imu::door_open_start_time = millis();
        }

        // Adds the values to the buffer if light sail hasn't deployed or the time hasn't been met
        if (!sfr::mission::deployed || ((millis() - sfr::imu::door_open_start_time) < constants::imu::door_open_end_time)) {

            // Checks if IMU Values are valid
            if (sfr::imu::gyro_x_value->get_value(&gyro_x) && sfr::imu::gyro_y_value->get_value(&gyro_y) && sfr::imu::gyro_z_value->get_value(&gyro_z)) {
                // Add gyro values to the buffer as rad/s
                sfr::imu::imu_dlink.push_front(NormalReportMonitor::serialize(sfr::imu::gyro_x_value));
                sfr::imu::imu_dlink.push_front(NormalReportMonitor::serialize(sfr::imu::gyro_y_value));
                sfr::imu::imu_dlink.push_front(NormalReportMonitor::serialize(sfr::imu::gyro_z_value));
            }

            // Delete older values if the dlink is larger then the maximum bytes downlinked
            if (sfr::imu::imu_dlink.size() > constants::rockblock::max_imu_report_length) {
                // If greater than the max size shrink the buffer
                for (int i = 0; i < 3; i++) {
                    sfr::imu::imu_dlink.pop_back();
                }
            }
            prev_deployed = sfr::mission::deployed;
        }

        // Turn IMU off if it has deployed and the time alloted time has finished.
        if (sfr::mission::deployed && (millis() - sfr::imu::door_open_start_time) > constants::imu::door_open_end_time) {
            sfr::imu::sample_gyro = false;
            sfr::imu::report_written = true;
        }
    }
}
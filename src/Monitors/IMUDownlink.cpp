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
            sfr::imu::power_setting = (uint8_t)sensor_power_mode_type::off;
            sfr::imu::report_written = true;
            values_unwritten = sfr::imu::imu_dlink.size();
        }
    }

    /* The following section will write one IMU fragment at a time to the SD card
       after all the IMU data has been collected until all data is saved. */

    if (sfr::imu::report_written && values_unwritten > 0) {
        // All IMU values have been collected post light sail deployment,
        // and there are still values to be written to the SD card

#ifdef VERBOSE
        Serial.print("Writing IMU fragment ");
        Serial.println(fragment_number);
#endif

        // Create file to write the fragment to the SD card
        String filename = "imu_frag_" + String(fragment_number) + ".txt";
        File txtFile = SD.open(filename.c_str(), FILE_WRITE);

        // Sets the amount of values that go into the report.
        int pop_size = min(constants::imu::max_gyro_imu_report_size, values_unwritten);

        // Write one fragment wroth of values from sfr::imu::imu_dlink, which holds all the collected IMU data, to the SD card
        for (int i = 0; i < pop_size; i++) {
            uint8_t data = sfr::imu::imu_dlink[start_index];
            txtFile.print(data, HEX);
            values_unwritten--;
            start_index++;
        }

        // Increment the fragment data member and close the fragment file on the SD card
        fragment_number++;
        txtFile.close();
    }
}
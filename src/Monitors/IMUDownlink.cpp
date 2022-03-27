#include "IMUDownlink.hpp"

IMUDownlink::IMUDownlink(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    imu = Adafruit_LSM9DS1(constants::imu::CSAG, constants::imu::CSM);
    imu.begin();
    // if(!imu.begin()) then...
}

void IMUDownlink::execute()
{
    if (sfr::mission::mode == mission_mode_type::mand_burns || sfr::mission::mode == mission_mode_type::reg_burns) {
        sensors_event_t accel, mag, gyro, temp;
        imu.getEvent(&accel, &mag, &gyro, &temp);

        imu.readMag();

        if (mag.magnetic.x > 4 || mag.magnetic.y > 4 || mag.magnetic.z > 4) {
            sfr::imu::imu_dlink_magid = imu_downlink_type::GAUSS_8;
        }
        if (mag.magnetic.x > 8 || mag.magnetic.y > 8 || mag.magnetic.z > 8) {
            sfr::imu::imu_dlink_magid = imu_downlink_type::GAUSS_12;
        }
        if (mag.magnetic.x > 12 || mag.magnetic.y > 12 || mag.magnetic.z > 12) {
            sfr::imu::imu_dlink_magid = imu_downlink_type::GAUSS_16;
        }

        // Get corrected IMU values
        imu.readGyro();

        imu.getEvent(&accel, &mag, &gyro, &temp);

        // Add reading to imu downlink buffer

        sfr::imu::imu_dlink_gyro_x_buffer.push_front(sfr::imu::gyro_x);
        sfr::imu::imu_dlink_gyro_y_buffer.push_front(sfr::imu::gyro_y);
        sfr::imu::imu_dlink_gyro_z_buffer.push_front(sfr::imu::gyro_z);

        // Remove old readings

        while (sfr::imu::imu_dlink_gyro_x_buffer.size() > sfr::imu::imu_downlink_buffer_max_size) {
            sfr::imu::imu_dlink_gyro_x_buffer.pop_back();
        }
        while (sfr::imu::imu_dlink_gyro_y_buffer.size() > sfr::imu::imu_downlink_buffer_max_size) {
            sfr::imu::imu_dlink_gyro_y_buffer.pop_back();
        }
        while (sfr::imu::imu_dlink_gyro_z_buffer.size() > sfr::imu::imu_downlink_buffer_max_size) {
            sfr::imu::imu_dlink_gyro_z_buffer.pop_back();
        }
        unsigned long imu_downlink_time_taken = millis() - sfr::imu::imu_downlink_start_time;
        if (imu_downlink_time_taken < constants::rockblock::min_downlink_period) {
            Serial.printf("IMU Downlink time execution time below expected");
        } else if (imu_downlink_time_taken > constants::rockblock::max_downlink_period) {
            sfr::imu::fragment_ready_start = true;
            sfr::imu::report_downlinked == true;
        }
    }
}
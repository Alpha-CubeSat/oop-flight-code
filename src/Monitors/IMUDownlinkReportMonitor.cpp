#include "IMUDownlinkReportMonitor.hpp"
unsigned int offset_copy;
IMUDownlinkReportMonitor::IMUDownlinkReportMonitor(unsigned int offset) : TimedControlTask<void>(offset)
{
    offset_copy = offset;
}

void IMUDownlinkReportMonitor::execute()
{
    IMUDownlink imu_dlink = IMUDownlink(offset_copy);
    imu_dlink.execute();

    int buffer_size = sfr::imu::imu_dlink_magid_buffer.size();
    for (int idx = 0; idx < buffer_size; ++idx) {
        // get the magid value from buffer and write to the imu downlink report
        uint8_t magid_value = (uint8_t)sfr::imu::imu_dlink_magid_buffer.back();
        sfr::imu::imu_dlink_magid_buffer.pop_back();
        sfr::imu::report[idx++] = magid_value;

        // get the gyro values from buffer and write to the imu downlink report
        float gyro_x_value = sfr::imu::imu_dlink_gyro_x_buffer.back();
        uint8_t gyro_x = map(gyro_x_value, constants::imu::min_gyro_x, constants::imu::max_gyro_x, 0, 255);
        // test_only uint8_t gyro_x = map(sfr::imu::gyro_x_average->get_value(), constants::imu::min_gyro_x, constants::imu::max_gyro_x, 0, 255);
        sfr::imu::imu_dlink_gyro_x_buffer.pop_back();
        sfr::imu::report[idx++] = gyro_x;

        float gyro_y_value = sfr::imu::imu_dlink_gyro_y_buffer.back();
        uint8_t gyro_y = map(gyro_y_value, constants::imu::min_gyro_y, constants::imu::max_gyro_y, 0, 255);
        // test_only uint8_t gyro_y = map(sfr::imu::gyro_y_average->get_value(), constants::imu::min_gyro_y, constants::imu::max_gyro_y, 0, 255);
        sfr::imu::imu_dlink_gyro_y_buffer.pop_back();
        sfr::imu::report[idx++] = gyro_y;

        float gyro_z_value = sfr::imu::imu_dlink_gyro_z_buffer.back();
        uint8_t gyro_z = map(gyro_z_value, constants::imu::min_gyro_z, constants::imu::max_gyro_z, 0, 255);
        // test_only uint8_t gyro_z = map(sfr::imu::gyro_z_average->get_value(), constants::imu::min_gyro_z, constants::imu::max_gyro_z, 0, 255);
        sfr::imu::imu_dlink_gyro_z_buffer.pop_back();
        sfr::imu::report[idx++] = gyro_z;

        // get the time stamp value from buffer and write to the imu downlink report
        uint8_t time_value = (uint8_t)sfr::imu::imu_dlink_time_buffer.back();
        sfr::imu::imu_dlink_time_buffer.pop_back();
        sfr::imu::report[idx++] = time_value;
    }
    sfr::imu::imu_dlink_report_ready = true;
}
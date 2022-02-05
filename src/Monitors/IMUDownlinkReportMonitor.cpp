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

    for (int idx = 0; idx < constants::sensor::collect; ++idx) {
        //get the magid value from buffer and write to the imu downlink report
        float magid_value = (float)sfr::imu::imu_dlink_magid_buffer.back();
        sfr::imu::imu_dlink_magid_buffer.pop_back();
        sfr::imu::report[idx++] = magid_value;

        //get the gyro values from buffer and write to the imu downlink report
        float gyro_x_value = sfr::imu::imu_dlink_gyro_x_buffer.back();
        sfr::imu::imu_dlink_gyro_x_buffer.pop_back();
        sfr::imu::report[idx++] = gyro_x_value;

        float gyro_y_value = sfr::imu::imu_dlink_gyro_y_buffer.back();
        sfr::imu::imu_dlink_gyro_y_buffer.pop_back();
        sfr::imu::report[idx++] = gyro_y_value;

        float gyro_z_value = sfr::imu::imu_dlink_gyro_z_buffer.back();
        sfr::imu::imu_dlink_gyro_z_buffer.pop_back();
        sfr::imu::report[idx++] = gyro_z_value;

        //get the time stamp value from buffer and write to the imu downlink report
        float time_value = (float)sfr::imu::imu_dlink_time_buffer.back();
        sfr::imu::imu_dlink_time_buffer.pop_back();
        sfr::imu::report[idx++] = time_value;
    }
}
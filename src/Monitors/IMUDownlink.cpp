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
    if (sfr::mission::current_mode == sfr::mission::mandatoryBurns || sfr::mission::current_mode == sfr::mission::regularBurns) {

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
    }
}
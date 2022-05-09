#include "IMUDownlink.hpp"

IMUDownlink::IMUDownlink(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void IMUDownlink::execute()
{
    if (sfr::imu::sample_gyro) {

        // Add reading to imu downlink buffer

        uint8_t gyro_x = map(sfr::imu::gyro_x_value->get_value(), sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
        uint8_t gyro_y = map(sfr::imu::gyro_y_value->get_value(), sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
        uint8_t gyro_z = map(sfr::imu::gyro_z_value->get_value(), sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);

        // sfr::imu::imu_dlink.push_front(sfr::imu::current_sample);
        sfr::imu::imu_dlink.push_front(gyro_x);
        sfr::imu::imu_dlink.push_front(gyro_y);
        sfr::imu::imu_dlink.push_front(gyro_z);

        // sfr::imu::current_sample++;
    }

    if (sfr::mission::current_mode == sfr::mission::mandatoryBurns || sfr::mission::current_mode == sfr::mission::regularBurns) {
        sfr::imu::sample_gyro = true;
    }

    // need to be stored in sfr later (time to record imu data after deployment)
    if (millis() - sfr::mission::time_deployed > 60 * constants::time::one_second) {
        sfr::imu::sample_gyro = false;
        sfr::imu::report_written = true;
    } else if (sfr::mission::deployed) {
        if (!sfr::mission::already_deployed) {
            sfr::mission::time_deployed = millis();
            sfr::mission::already_deployed = true;
        }
        sfr::imu::sample_gyro = true;
    }
}

#include "IMUDownlink.hpp"

IMUDownlink::IMUDownlink(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void IMUDownlink::execute()
{
    float gyro_x;
    float gyro_y;
    float gyro_z;

    if (sfr::imu::sample_gyro) {

        // Add reading to imu downlink buffer

        sfr::imu::gyro_x_value->get_value(&gyro_x);
        sfr::imu::gyro_x_value->get_value(&gyro_y);
        sfr::imu::gyro_x_value->get_value(&gyro_z);

        sfr::imu::imu_dlink.push_front(gyro_x);
        sfr::imu::imu_dlink.push_front(gyro_y);
        sfr::imu::imu_dlink.push_front(gyro_z);
    }

    if (sfr::mission::current_mode->get_id() == sfr::mission::mandatoryBurns->get_id() || sfr::mission::current_mode->get_id() == sfr::mission::regularBurns->get_id()) {
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

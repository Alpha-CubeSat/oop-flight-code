#include "IMUDownlink.hpp"

IMUDownlink::IMUDownlink(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    imu = Adafruit_LSM9DS1(constants::imu::CSAG, constants::imu::CSM);
    imu.begin();
}

void IMUDownlink::execute()
{
    // if (sfr::mission::current_mode == sfr::mission::mandatoryBurns || sfr::mission::current_mode == sfr::mission::regularBurns) {
    if (millis() - sfr::imu::start_time_deployed > 60 * constants::time::one_second) {
        sfr::mission::deployed = true;
    }
    if (sfr::mission::deployed == true) {
        sfr::rockblock::imu_downlink_on = true;
        if (sfr::rockblock::imu_first_start == true) {
            sfr::rockblock::imu_first_start = false;
            sfr::rockblock::imudownlink_start_time = millis();
        }

        if (sfr::rockblock::imu_downlink_on == true) {

            sfr::imu::gyro_x = sfr::imu::gyro_x_value->get_value();
            sfr::imu::gyro_y = sfr::imu::gyro_y_value->get_value();
            sfr::imu::gyro_z = sfr::imu::gyro_z_value->get_value();

            // Add reading to imu downlink buffer
            sfr::imu::imu_dlink_gyro_x_buffer.push_front(sfr::imu::gyro_x);
            sfr::imu::imu_dlink_gyro_y_buffer.push_front(sfr::imu::gyro_y);
            sfr::imu::imu_dlink_gyro_z_buffer.push_front(sfr::imu::gyro_z);

            if (millis() - sfr::rockblock::imudownlink_start_time > 60 * constants::time::one_second) {
                sfr::rockblock::imu_downlink_on = false;
                sfr::imu::data_downlinked = true;
            }
        }
    }
}

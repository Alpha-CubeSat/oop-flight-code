#include "NormalReportMonitor.hpp"

NormalReportMonitor::NormalReportMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void NormalReportMonitor::execute(){
    uint8_t mag_x = map(sfr::imu::mag_x_average, constants::imu::min_mag_x, constants::imu::max_mag_x, 0, 255);
    uint8_t mag_y = map(sfr::imu::mag_y_average, constants::imu::min_mag_y, constants::imu::max_mag_y, 0, 255);
    uint8_t mag_z = map(sfr::imu::mag_z_average, constants::imu::min_mag_z, constants::imu::max_mag_z, 0, 255);

    uint8_t gyro_x = map(sfr::imu::gyro_x_average, constants::imu::min_gyro_x, constants::imu::max_gyro_x, 0, 255);
    uint8_t gyro_y = map(sfr::imu::gyro_y_average, constants::imu::min_gyro_y, constants::imu::max_gyro_y, 0, 255);
    uint8_t gyro_z = map(sfr::imu::gyro_z_average, constants::imu::min_gyro_z, constants::imu::max_gyro_z, 0, 255);

    uint8_t temp_c = map(sfr::temperature::temp_c_average, constants::temperature::min_temp_c, constants::temperature::max_temp_c, 0, 255);

    uint8_t solar_current = map(sfr::current::solar_current_average, constants::current::min_solar_current, constants::current::max_solar_current, 0, 255);

    uint8_t voltage = map(sfr::battery::voltage_average, constants::battery::min_voltage, constants::battery::max_voltage, 0, 255);

    uint8_t downlink_period = map(sfr::rockblock::downlink_period, constants::rockblock::min_downlink_period, constants::rockblock::max_downlink_period, 0, 255);

    sfr::rockblock::report[0] = sfr::photoresistor::covered;
    sfr::rockblock::report[1] = sfr::button::pressed;
    sfr::rockblock::report[2] = (uint8_t) sfr::mission::mode;
    sfr::rockblock::report[3] = sfr::burnwire::fire;
    sfr::rockblock::report[4] = sfr::burnwire::arm;
    sfr::rockblock::report[5] = sfr::burnwire::burn_time;
    sfr::rockblock::report[6] = sfr::burnwire::armed_time;
    sfr::rockblock::report[7] = (uint8_t) sfr::burnwire::mode;
    sfr::rockblock::report[8] = sfr::burnwire::attempts;
    sfr::rockblock::report[9] = downlink_period;
    sfr::rockblock::report[10] = sfr::rockblock::waiting_message;
    sfr::rockblock::report[11] = sfr::rockblock::waiting_command;
    sfr::rockblock::report[12] = mag_x;
    sfr::rockblock::report[13] = mag_y;
    sfr::rockblock::report[14] = mag_z;
    sfr::rockblock::report[15] = gyro_x;
    sfr::rockblock::report[16] = gyro_y;
    sfr::rockblock::report[17] = gyro_z;
    sfr::rockblock::report[18] = temp_c;
    sfr::rockblock::report[19] = (uint8_t) sfr::temperature::mode;
    sfr::rockblock::report[20] = solar_current;
    sfr::rockblock::report[21] = sfr::current::in_sun;
    sfr::rockblock::report[22] = (uint8_t) sfr::acs::mode;
    sfr::rockblock::report[23] = voltage;
    sfr::rockblock::report[24] = (uint8_t) sfr::fault::mode;
    sfr::rockblock::report[25] = sfr::fault::check_mag_x;
    sfr::rockblock::report[26] = sfr::fault::check_mag_y;
    sfr::rockblock::report[27] = sfr::fault::check_mag_z;
    sfr::rockblock::report[28] = sfr::fault::check_gyro_x;
    sfr::rockblock::report[29] = sfr::fault::check_gyro_y;
    sfr::rockblock::report[30] = sfr::fault::check_gyro_z;
    sfr::rockblock::report[31] = sfr::fault::check_temp_c;
    sfr::rockblock::report[32] = sfr::fault::check_voltage;
    sfr::rockblock::report[33] = sfr::fault::check_solar_current;
    sfr::rockblock::report[34] = sfr::camera::take_photo;
    sfr::rockblock::report[35] = sfr::camera::powered;
}
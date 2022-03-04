#include "NormalReportMonitor.hpp"

NormalReportMonitor::NormalReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void NormalReportMonitor::execute()
{
    uint8_t mag_x = map(sfr::imu::mag_x_average->get_value(), constants::imu::min_mag_x, constants::imu::max_mag_x, 0, 255);
    uint8_t mag_y = map(sfr::imu::mag_y_average->get_value(), constants::imu::min_mag_y, constants::imu::max_mag_y, 0, 255);
    uint8_t mag_z = map(sfr::imu::mag_z_average->get_value(), constants::imu::min_mag_z, constants::imu::max_mag_z, 0, 255);

    uint8_t gyro_x = map(sfr::imu::gyro_x_average->get_value(), constants::imu::min_gyro_x, constants::imu::max_gyro_x, 0, 255);
    uint8_t gyro_y = map(sfr::imu::gyro_y_average->get_value(), constants::imu::min_gyro_y, constants::imu::max_gyro_y, 0, 255);
    uint8_t gyro_z = map(sfr::imu::gyro_z_average->get_value(), constants::imu::min_gyro_z, constants::imu::max_gyro_z, 0, 255);

    uint8_t temp_c = map(sfr::temperature::temp_c_average->get_value(), constants::temperature::min_temp_c, constants::temperature::max_temp_c, 0, 255);

    // TODO: finalize min/max
    uint8_t solar_current = map(sfr::current::solar_current_average->get_value(), constants::current::min_solar_current, constants::current::max_solar_current, 0, 255);

    uint8_t voltage = map(sfr::battery::voltage_average->get_value(), constants::battery::min_voltage, constants::battery::max_voltage, 0, 255);

    uint8_t downlink_period = map(sfr::rockblock::downlink_period, constants::rockblock::min_downlink_period, constants::rockblock::max_downlink_period, 0, 255);

    sfr::rockblock::normal_report[0] = 21;
    sfr::rockblock::normal_report[1] = sfr::photoresistor::covered;
    sfr::rockblock::normal_report[2] = sfr::button::pressed;
    sfr::rockblock::normal_report[3] = (uint8_t)sfr::mission::mode;
    sfr::rockblock::normal_report[4] = sfr::burnwire::fire;
    sfr::rockblock::normal_report[5] = sfr::burnwire::arm;
    sfr::rockblock::normal_report[6] = sfr::burnwire::burn_time;
    sfr::rockblock::normal_report[7] = sfr::burnwire::armed_time;
    sfr::rockblock::normal_report[8] = (uint8_t)sfr::burnwire::mode;
    sfr::rockblock::normal_report[9] = sfr::burnwire::attempts;
    sfr::rockblock::normal_report[10] = downlink_period;
    sfr::rockblock::normal_report[11] = sfr::rockblock::waiting_message;
    sfr::rockblock::normal_report[12] = sfr::rockblock::waiting_command;
    sfr::rockblock::normal_report[13] = mag_x;
    sfr::rockblock::normal_report[14] = mag_y;
    sfr::rockblock::normal_report[15] = mag_z;
    sfr::rockblock::normal_report[16] = gyro_x;
    sfr::rockblock::normal_report[17] = gyro_y;
    sfr::rockblock::normal_report[18] = gyro_z;
    sfr::rockblock::normal_report[19] = temp_c;
    // removed temperature mode type
    sfr::rockblock::normal_report[21] = solar_current;
    sfr::rockblock::normal_report[22] = sfr::current::in_sun;
    sfr::rockblock::normal_report[23] = (uint8_t)sfr::acs::mode;
    sfr::rockblock::normal_report[24] = voltage;
    sfr::rockblock::normal_report[25] = (uint8_t)sfr::fault::mode;
    sfr::rockblock::normal_report[26] = sfr::imu::mag_x_average->is_valid();
    sfr::rockblock::normal_report[27] = sfr::imu::mag_y_average->is_valid();
    sfr::rockblock::normal_report[28] = sfr::imu::mag_z_average->is_valid();
    sfr::rockblock::normal_report[29] = sfr::imu::gyro_x_average->is_valid();
    sfr::rockblock::normal_report[30] = sfr::imu::gyro_y_average->is_valid();
    sfr::rockblock::normal_report[31] = sfr::imu::gyro_z_average->is_valid();
    sfr::rockblock::normal_report[32] = sfr::temperature::temp_c_average->is_valid();
    sfr::rockblock::normal_report[33] = sfr::battery::voltage_average->is_valid();
    sfr::rockblock::normal_report[34] = sfr::current::solar_current_average->is_valid();
    sfr::rockblock::normal_report[35] = sfr::camera::take_photo;
    sfr::rockblock::normal_report[36] = sfr::camera::powered;
}
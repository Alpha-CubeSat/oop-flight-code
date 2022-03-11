#include "NormalReportMonitor.hpp"

NormalReportMonitor::NormalReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

std::queue<uint8_t> NormalReportMonitor::commands_received;

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

    sfr::rockblock::report[0] = constants::rockblock::start_of_downlink_flag;
    sfr::rockblock::report[1] = sfr::photoresistor::covered;
    sfr::rockblock::report[2] = sfr::button::pressed;
    sfr::rockblock::report[3] = (uint8_t)sfr::mission::mode;
    sfr::rockblock::report[4] = sfr::burnwire::fire;
    sfr::rockblock::report[5] = sfr::burnwire::arm;
    sfr::rockblock::report[6] = sfr::burnwire::burn_time;
    sfr::rockblock::report[7] = sfr::burnwire::armed_time;
    sfr::rockblock::report[8] = (uint8_t)sfr::burnwire::mode;
    sfr::rockblock::report[9] = sfr::burnwire::attempts;
    sfr::rockblock::report[10] = downlink_period;
    sfr::rockblock::report[11] = sfr::rockblock::waiting_message;
    sfr::rockblock::report[12] = sfr::rockblock::waiting_command;
    sfr::rockblock::report[13] = mag_x;
    sfr::rockblock::report[14] = mag_y;
    sfr::rockblock::report[15] = mag_z;
    sfr::rockblock::report[16] = gyro_x;
    sfr::rockblock::report[17] = gyro_y;
    sfr::rockblock::report[18] = gyro_z;
    sfr::rockblock::report[19] = temp_c;
    // removed temperature mode type
    sfr::rockblock::report[21] = solar_current;
    sfr::rockblock::report[22] = sfr::current::in_sun;
    sfr::rockblock::report[23] = (uint8_t)sfr::acs::mode;
    sfr::rockblock::report[24] = voltage;
    sfr::rockblock::report[25] = (uint8_t)sfr::fault::mode;
    sfr::rockblock::report[26] = sfr::imu::mag_x_average->is_valid();
    sfr::rockblock::report[27] = sfr::imu::mag_y_average->is_valid();
    sfr::rockblock::report[28] = sfr::imu::mag_z_average->is_valid();
    sfr::rockblock::report[29] = sfr::imu::gyro_x_average->is_valid();
    sfr::rockblock::report[30] = sfr::imu::gyro_y_average->is_valid();
    sfr::rockblock::report[31] = sfr::imu::gyro_z_average->is_valid();
    sfr::rockblock::report[32] = sfr::temperature::temp_c_average->is_valid();
    sfr::rockblock::report[33] = sfr::battery::voltage_average->is_valid();
    sfr::rockblock::report[34] = sfr::current::solar_current_average->is_valid();
    sfr::rockblock::report[35] = sfr::camera::take_photo;
    sfr::rockblock::report[36] = sfr::camera::powered;
    int index = 37;
    while (!commands_received.empty() && index < 67) {
        sfr::rockblock::report[index] = commands_received.front();
        commands_received.pop();
        index++;
    } // Writes opcodes to normal report; two indexes constitute one opcode since each opcode is 2-byte
    sfr::rockblock::report[index] = constants::rockblock::end_of_downlink_flag;
}
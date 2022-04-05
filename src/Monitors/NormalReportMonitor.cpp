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

    sfr::rockblock::normal_report.clear();
    sfr::rockblock::normal_report.push_back(constants::rockblock::start_of_normal_downlink_flag);
    sfr::rockblock::normal_report.push_back(sfr::photoresistor::covered);
    sfr::rockblock::normal_report.push_back(sfr::button::pressed);
    sfr::rockblock::normal_report.push_back(sfr::mission::current_mode->id());
    sfr::rockblock::normal_report.push_back(sfr::burnwire::fire);
    sfr::rockblock::normal_report.push_back(sfr::burnwire::arm);
    sfr::rockblock::normal_report.push_back(sfr::burnwire::burn_time);
    sfr::rockblock::normal_report.push_back(sfr::burnwire::armed_time);
    sfr::rockblock::normal_report.push_back((uint8_t)sfr::burnwire::mode);
    sfr::rockblock::normal_report.push_back(sfr::burnwire::attempts);
    sfr::rockblock::normal_report.push_back(downlink_period);
    sfr::rockblock::normal_report.push_back(sfr::rockblock::waiting_message);
    sfr::rockblock::normal_report.push_back(sfr::rockblock::waiting_command);
    sfr::rockblock::normal_report.push_back(mag_x);
    sfr::rockblock::normal_report.push_back(mag_y);
    sfr::rockblock::normal_report.push_back(mag_z);
    sfr::rockblock::normal_report.push_back(gyro_x);
    sfr::rockblock::normal_report.push_back(gyro_y);
    sfr::rockblock::normal_report.push_back(gyro_z);
    sfr::rockblock::normal_report.push_back(temp_c);
    // removed temperature mode type
    sfr::rockblock::normal_report.push_back(solar_current);
    sfr::rockblock::normal_report.push_back(sfr::current::in_sun);
    sfr::rockblock::normal_report.push_back((uint8_t)sfr::acs::mode);
    sfr::rockblock::normal_report.push_back(voltage);
    sfr::rockblock::normal_report.push_back((uint8_t)sfr::fault::mode);
    sfr::rockblock::normal_report.push_back(sfr::imu::mag_x_average->is_valid());
    sfr::rockblock::normal_report.push_back(sfr::imu::mag_y_average->is_valid());
    sfr::rockblock::normal_report.push_back(sfr::imu::mag_z_average->is_valid());
    sfr::rockblock::normal_report.push_back(sfr::imu::gyro_x_average->is_valid());
    sfr::rockblock::normal_report.push_back(sfr::imu::gyro_y_average->is_valid());
    sfr::rockblock::normal_report.push_back(sfr::imu::gyro_z_average->is_valid());
    sfr::rockblock::normal_report.push_back(sfr::temperature::temp_c_average->is_valid());
    sfr::rockblock::normal_report.push_back(sfr::battery::voltage_average->is_valid());
    sfr::rockblock::normal_report.push_back(sfr::current::solar_current_average->is_valid());
    sfr::rockblock::normal_report.push_back(sfr::camera::take_photo);
    sfr::rockblock::normal_report.push_back(sfr::camera::powered);
    int i = 0;
    while (!commands_received.empty() && i < 30) {
        sfr::rockblock::normal_report.push_back(commands_received.front());
        commands_received.pop();
        ++i;
    } // Writes opcodes to normal report; two indices constitute one opcode since each opcode is 2-byte
    std::queue<uint8_t> empty;
    std::swap(commands_received, empty); // Clear the queue after each normal report is generated
    sfr::rockblock::normal_report.push_back(constants::rockblock::end_of_normal_downlink_flag1);
    sfr::rockblock::normal_report.push_back(constants::rockblock::end_of_normal_downlink_flag2);
}
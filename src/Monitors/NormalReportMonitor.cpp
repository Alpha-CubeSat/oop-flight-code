#include "NormalReportMonitor.hpp"
#include "sfr.hpp"

NormalReportMonitor::NormalReportMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

std::queue<uint8_t> NormalReportMonitor::commands_received;
std::map<uint8_t, std::vector<int>> mapping_bounds = {
    // normal report index: min, max
    {4, {0, 5000}},
    {5, {0, 864000000}},
    {8, {60000, 172800000}},
    {11, {-150, 150}},
    {12, {-150, 150}},
    {13, {-150, 150}},
    {14, {-5, 5}},
    {15, {-5, 5}},
    {16, {-5, 5}},
    {17, {0, 1023}},
    {18, {-50, 200}},
    {21, {3, 5}},
};

void NormalReportMonitor::execute()
{
    float mag_x;
    float mag_y;
    float mag_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float photoresistor;
    float temp;
    float current;
    float voltage;

    sfr::imu::mag_x_average->get_value(&mag_x);
    sfr::imu::mag_x_average->get_value(&mag_y);
    sfr::imu::mag_x_average->get_value(&mag_z);
    sfr::imu::mag_x_average->get_value(&gyro_x);
    sfr::imu::mag_x_average->get_value(&gyro_y);
    sfr::imu::mag_x_average->get_value(&gyro_z);
    sfr::photoresistor::light_val_average_deployment->get_value(&photoresistor);
    sfr::temperature::temp_c_value->get_value(&temp);
    sfr::current::solar_current_average->get_value(&current);
    sfr::battery::voltage_average->get_value(&voltage);

    std::map<uint8_t, uint8_t> report_contents = {
        {0, 99},
        {1, (uint8_t)sfr::photoresistor::covered},
        {2, (uint8_t)sfr::button::pressed},
        {3, (uint8_t)sfr::mission::current_mode->get_id()},
        {4, serialize(4, sfr::burnwire::burn_time)},
        {5, serialize(5, sfr::burnwire::armed_time)},
        {6, (uint8_t)sfr::burnwire::mode},
        {7, (uint8_t)sfr::burnwire::attempts},
        {8, serialize(8, sfr::rockblock::downlink_period)},
        {9, (uint8_t)sfr::rockblock::waiting_message},
        {10, (uint8_t)sfr::rockblock::waiting_command},
        {11, serialize(11, mag_x)},
        {12, serialize(12, mag_y)},
        {13, serialize(13, mag_z)},
        {14, serialize(14, gyro_x)},
        {15, serialize(15, gyro_y)},
        {16, serialize(16, gyro_z)},
        {17, serialize(17, photoresistor)},
        {18, serialize(18, temp)},
        {19, serialize(19, current)},
        {20, (uint8_t)sfr::current::in_sun},
        {21, serialize(21, voltage)},
        {22, (uint8_t)faults::fault_1},
        {23, (uint8_t)faults::fault_2},
        {24, (uint8_t)faults::fault_3},
        {25, (uint8_t)sfr::camera::powered},
        {26, 0}, // number of reboots (pending EEPROM implementation)
        {27, (uint8_t)(sfr::imu::mag_x_average->is_valid() | (sfr::imu::mag_y_average->is_valid() << 1) | (sfr::imu::mag_z_average->is_valid() << 2) | (sfr::imu::gyro_x_average->is_valid() << 3) |
                       (sfr::imu::gyro_y_average->is_valid() << 4) | (sfr::imu::gyro_z_average->is_valid() << 5) | (sfr::temperature::temp_c_average->is_valid() << 6) |
                       (sfr::current::solar_current_average->is_valid() << 7))},
        {28, (uint8_t)(sfr::battery::voltage_average->is_valid() | (sfr::photoresistor::light_val_average_standby->is_valid() << 1))},
    };

    sfr::rockblock::normal_report.clear();
    for (int i = 0; i <= 27; i++) {
        sfr::rockblock::normal_report.push_back(report_contents[i]);
    }

    /*duncan's pr will modify this

    int i = 0;
    while (!commands_received.empty() && i < 30) {
        sfr::rockblock::normal_report.push_back(commands_received.front());
        // sfr::rockblock::normal_report.pop_front();
        commands_received.pop();
        ++i;
    } // Writes opcodes to normal report; two indices constitute one opcode since each opcode is 2-byte
    std::queue<uint8_t> empty;
    std::swap(commands_received, empty); // Clear the queue after each normal report is generated

    */
    // duncan's pr will modify this

    // Serial.println(sfr::rockblock::normal_report.size());
    sfr::rockblock::normal_report.push_back(constants::rockblock::end_of_normal_downlink_flag1);
    sfr::rockblock::normal_report.push_back(constants::rockblock::end_of_normal_downlink_flag2);
}

uint8_t NormalReportMonitor::serialize(int index, int value)
{
    return round((value - mapping_bounds[index][0]) * (255 / (mapping_bounds[index][1] - mapping_bounds[index][0])));
}
#include "NormalReportMonitor.hpp"
#include "Faults.hpp"
#include "sfr.hpp"

NormalReportMonitor::NormalReportMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

std::map<uint8_t, std::vector<float>> mapping_bounds = {
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
    sfr::imu::mag_y_average->get_value(&mag_y);
    sfr::imu::mag_z_average->get_value(&mag_z);
    sfr::imu::gyro_x_average->get_value(&gyro_x);
    sfr::imu::gyro_y_average->get_value(&gyro_y);
    sfr::imu::gyro_z_average->get_value(&gyro_z);
    sfr::photoresistor::light_val_average_deployment->get_value(&photoresistor);
    sfr::temperature::temp_c_average->get_value(&temp);
    sfr::current::solar_current_average->get_value(&current);
    sfr::battery::voltage_average->get_value(&voltage);

    std::map<uint8_t, uint8_t> report_contents = {
        {0, 99},
        {1, sfr::photoresistor::covered},
        {2, sfr::button::pressed},
        {3, sfr::mission::current_mode->get_id()},

        // TODO

        // All SensorReading values. If there is an average version, that is included.

        // All SensorReading Faults
        {22, (fault_groups::imu_faults::mag_x_value->serialize() << 4) + fault_groups::imu_faults::mag_x_average->serialize()},
        {23, (fault_groups::imu_faults::mag_y_value->serialize() << 4) + fault_groups::imu_faults::mag_y_average->serialize()},
        {24, (fault_groups::imu_faults::mag_z_value->serialize() << 4) + fault_groups::imu_faults::mag_z_average->serialize()},
        {25, (fault_groups::imu_faults::gyro_x_value->serialize() << 4) + fault_groups::imu_faults::gyro_x_average->serialize()},
        {26, (fault_groups::imu_faults::gyro_y_value->serialize() << 4) + fault_groups::imu_faults::gyro_y_average->serialize()},
        {27, (fault_groups::imu_faults::gyro_z_value->serialize() << 4) + fault_groups::imu_faults::gyro_z_average->serialize()},
        {28, (fault_groups::imu_faults::acc_x_average->serialize() << 4) + fault_groups::imu_faults::acc_y_average->serialize()},
        {29, (fault_groups::power_faults::temp_c_value->serialize() << 4) + fault_groups::power_faults::temp_c_average->serialize()},
        {30, (fault_groups::power_faults::voltage_value->serialize() << 4) + fault_groups::power_faults::voltage_average->serialize()},
        {31, (fault_groups::power_faults::solar_current_average->serialize() << 4)},

        {32, (fault_groups::hardware_faults::light_val->serialize() << 4) + fault_groups::hardware_faults::button->serialize()},
    };

    std::deque<uint8_t> empty_normal_report;
    std::swap(sfr::rockblock::normal_report, empty_normal_report);

    for (int i = 0; i < report_contents.size(); i++) {
        sfr::rockblock::normal_report.push_back(report_contents[i]);
    }

    // push the most recent 20 mission modes switches, starting with most recent
    int k = 0;
    auto hist_mode = sfr::mission::mode_history.cbegin();
    auto prev_hist_mode = sfr::mission::mode_history.cbegin();
    while (hist_mode != sfr::mission::mode_history.cend() && k <= sfr::mission::mission_mode_hist_length) {
        if (*hist_mode != *prev_hist_mode || hist_mode == sfr::mission::mode_history.cbegin()) {
            sfr::rockblock::normal_report.push_back(*hist_mode);
            k++;
        }
        if (hist_mode != sfr::mission::mode_history.cbegin()) {
            std::advance(prev_hist_mode, 1);
        }
        std::advance(hist_mode, 1);
    }

    // add delimeter between mission mode history and processed opcodes
    sfr::rockblock::normal_report.push_back(constants::rockblock::normal_report_delimiter);

    int j = 0;
    auto current_command = sfr::rockblock::commands_received.cbegin();
    while (current_command != sfr::rockblock::commands_received.cend() && j < sfr::rockblock::normal_report_command_max) {
        sfr::rockblock::normal_report.push_back(*current_command);
        std::advance(current_command, 1);
        j++;
    }
    sfr::rockblock::normal_report_command_curr = j;
    sfr::rockblock::normal_report.push_back(constants::rockblock::end_of_normal_downlink_flag1);
    sfr::rockblock::normal_report.push_back(constants::rockblock::end_of_normal_downlink_flag2);
}

uint8_t NormalReportMonitor::serialize(int index, float value)
{
    return round(value - mapping_bounds[index][0]) * (255 / (mapping_bounds[index][1] - mapping_bounds[index][0]));
}

uint8_t NormalReportMonitor::serialize(bool values[])
{
    uint8_t result = 0;

    for (int i = 0; i < 8; i++) {
        if (values[i]) {
            result |= (1 << i);
        }
    }

    return result;
}

#include "NormalReportMonitor.hpp"
#include "Faults.hpp"
#include "sfr.hpp"

NormalReportMonitor::NormalReportMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void NormalReportMonitor::execute()
{
    float light_avg_standby;
    float light_avg_deploy;
    float mag_x;
    float mag_y;
    float mag_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float mag_x_avg;
    float mag_y_avg;
    float mag_z_avg;
    float gyro_x_avg;
    float gyro_y_avg;
    float gyro_z_avg;
    float temp;
    float temp_avg;
    float current_avg;
    float voltage;
    float voltage_avg;

    sfr::photoresistor::light_val_average_standby->get_value(&light_avg_standby);
    sfr::photoresistor::light_val_average_deployment->get_value(&light_avg_deploy);
    sfr::imu::mag_x_value->get_value(&mag_x);
    sfr::imu::mag_y_value->get_value(&mag_y);
    sfr::imu::mag_z_value->get_value(&mag_z);
    sfr::imu::gyro_x_value->get_value(&gyro_x);
    sfr::imu::gyro_y_value->get_value(&gyro_y);
    sfr::imu::gyro_z_value->get_value(&gyro_z);
    sfr::imu::mag_x_average->get_value(&mag_x_avg);
    sfr::imu::mag_y_average->get_value(&mag_y_avg);
    sfr::imu::mag_z_average->get_value(&mag_z_avg);
    sfr::imu::gyro_x_average->get_value(&gyro_x_avg);
    sfr::imu::gyro_y_average->get_value(&gyro_y_avg);
    sfr::imu::gyro_z_average->get_value(&gyro_z_avg);
    sfr::temperature::temp_c_value->get_value(&temp);
    sfr::temperature::temp_c_average->get_value(&temp_avg);
    sfr::current::solar_current_average->get_value(&current_avg);
    sfr::battery::voltage_value->get_value(&voltage);
    sfr::battery::voltage_average->get_value(&voltage_avg);

    bool sfr_packed_bools[] = {
        sfr::photoresistor::covered, sfr::mission::possible_uncovered, sfr::camera::powered,
        sfr::rockblock::waiting_message, sfr::rockblock::waiting_command,
        sfr::temperature::in_sun, sfr::current::in_sun, sfr::button::pressed};

    std::map<uint8_t, uint8_t> report_contents = {
        {0, 99},
        // SFR fields
        {1, serialize(sfr::burnwire::burn_time, 0, 5000)},
        {2, serialize(sfr::burnwire::armed_time, 0, 864000000)},
        {3, serialize(sfr::rockblock::downlink_period, 60000, 172800000)},
        {4, (uint8_t)sfr::eeprom::boot_counter},
        {5, serialize(sfr_packed_bools)},

        // Sensor readings
        {6, serialize(light_avg_standby, 0, 1000)},
        {7, serialize(light_avg_deploy, 0, 1000)},
        {8, serialize(mag_x, -100, 100)},
        {9, serialize(mag_y, -100, 100)},
        {10, serialize(mag_z, -100, 100)},
        {11, serialize(gyro_x, -100, 100)},
        {12, serialize(gyro_y, -100, 100)},
        {13, serialize(gyro_z, -100, 100)},
        {14, serialize(mag_x_avg, -100, 100)},
        {15, serialize(mag_y_avg, -100, 100)},
        {16, serialize(mag_z_avg, -100, 100)},
        {17, serialize(gyro_x_avg, -100, 100)},
        {18, serialize(gyro_y_avg, -100, 100)},
        {19, serialize(gyro_z_avg, -100, 100)},
        {20, serialize(temp, -500, 500)},
        {21, serialize(temp_avg, -100, 200)},
        {22, serialize(current_avg, -75, 500)},
        {23, serialize(voltage, 0, 5.5)},
        {24, serialize(voltage_avg, 0, 5.5)},

        // Faults
        {25, (fault_groups::imu_faults::mag_x_value->serialize() << 4) + fault_groups::imu_faults::mag_x_average->serialize()},
        {26, (fault_groups::imu_faults::mag_y_value->serialize() << 4) + fault_groups::imu_faults::mag_y_average->serialize()},
        {27, (fault_groups::imu_faults::mag_z_value->serialize() << 4) + fault_groups::imu_faults::mag_z_average->serialize()},
        {28, (fault_groups::imu_faults::gyro_x_value->serialize() << 4) + fault_groups::imu_faults::gyro_x_average->serialize()},
        {29, (fault_groups::imu_faults::gyro_y_value->serialize() << 4) + fault_groups::imu_faults::gyro_y_average->serialize()},
        {30, (fault_groups::imu_faults::gyro_z_value->serialize() << 4) + fault_groups::imu_faults::gyro_z_average->serialize()},
        {31, (fault_groups::power_faults::temp_c_value->serialize() << 4) + fault_groups::power_faults::temp_c_average->serialize()},
        {32, (fault_groups::power_faults::voltage_value->serialize() << 4) + fault_groups::power_faults::voltage_average->serialize()},
        {33, (fault_groups::power_faults::solar_current_average->serialize() << 4)},
        {34, (fault_groups::hardware_faults::light_val->serialize() << 4) + fault_groups::hardware_faults::button->serialize()}};

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

uint8_t NormalReportMonitor::serialize(float value, float min, float max)
{
    return round(value - min) * (255 / (max - min));
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

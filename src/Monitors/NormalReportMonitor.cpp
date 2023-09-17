#include "NormalReportMonitor.hpp"
#include "Faults.hpp"
#include "sfr.hpp"

NormalReportMonitor::NormalReportMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void NormalReportMonitor::execute()
{

    bool sfr_packed_bools[] = {
        sfr::photoresistor::covered, sfr::mission::possible_uncovered, sfr::camera::powered,
        sfr::rockblock::waiting_message, sfr::rockblock::waiting_command,
        sfr::temperature::in_sun, sfr::current::in_sun, sfr::button::pressed};

    std::vector<uint8_t> report_contents{
        99,
        // SFR fields
        serialize(sfr::burnwire::burn_time),
        serialize(sfr::burnwire::armed_time),
        serialize(sfr::rockblock::downlink_period),
        sfr::eeprom::boot_counter,
        serialize(sfr::acs::Id_index),
        serialize(sfr::acs::Kd_index),
        serialize(sfr::acs::Kp_index),
        serialize(sfr::acs::c_index),
        serialize(sfr_packed_bools),

        // Sensor readings
        serialize(sfr::photoresistor::light_val_average_standby),
        serialize(sfr::photoresistor::light_val_average_deployment),
        serialize(sfr::imu::mag_x_value),
        serialize(sfr::imu::mag_y_value),
        serialize(sfr::imu::mag_z_value),
        serialize(sfr::imu::gyro_x_value),
        serialize(sfr::imu::gyro_y_value),
        serialize(sfr::imu::gyro_z_value),
        serialize(sfr::imu::mag_x_average),
        serialize(sfr::imu::mag_y_average),
        serialize(sfr::imu::mag_z_average),
        serialize(sfr::imu::gyro_x_average),
        serialize(sfr::imu::gyro_y_average),
        serialize(sfr::imu::gyro_z_average),
        serialize(sfr::temperature::temp_c_value),
        serialize(sfr::temperature::temp_c_average),
        serialize(sfr::current::solar_current_average),
        serialize(sfr::battery::voltage_value),
        serialize(sfr::battery::voltage_average),

        // Faults
        (uint8_t)((fault_groups::imu_faults::mag_x_value->serialize() << 4) + fault_groups::imu_faults::mag_x_average->serialize()),
        (uint8_t)((fault_groups::imu_faults::mag_y_value->serialize() << 4) + fault_groups::imu_faults::mag_y_average->serialize()),
        (uint8_t)((fault_groups::imu_faults::mag_z_value->serialize() << 4) + fault_groups::imu_faults::mag_z_average->serialize()),
        (uint8_t)((fault_groups::imu_faults::gyro_x_value->serialize() << 4) + fault_groups::imu_faults::gyro_x_average->serialize()),
        (uint8_t)((fault_groups::imu_faults::gyro_y_value->serialize() << 4) + fault_groups::imu_faults::gyro_y_average->serialize()),
        (uint8_t)((fault_groups::imu_faults::gyro_z_value->serialize() << 4) + fault_groups::imu_faults::gyro_z_average->serialize()),
        (uint8_t)((fault_groups::power_faults::temp_c_value->serialize() << 4) + fault_groups::power_faults::temp_c_average->serialize()),
        (uint8_t)((fault_groups::power_faults::voltage_value->serialize() << 4) + fault_groups::power_faults::voltage_average->serialize()),
        (uint8_t)((fault_groups::power_faults::solar_current_average->serialize() << 4)),
        (uint8_t)((fault_groups::hardware_faults::light_val->serialize() << 4) + fault_groups::hardware_faults::button->serialize())};

    std::deque<uint8_t> empty_normal_report;
    std::swap(sfr::rockblock::normal_report, empty_normal_report);

    for (size_t i = 0; i < report_contents.size(); i++) {
        sfr::rockblock::normal_report.push_back(report_contents[i]);
    }

    // TODO: ensure mission mode history is constant length and pack into 10 bytes

    // push the most recent 16 mission modes switches, starting with most recent
    size_t k = 0;
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

    // push the most recent 10 command opcodes, starting with most recent
    // if less than 10 commands have been recieved, fill with empty opcodes
    size_t commands = 0;
    auto current_command = sfr::rockblock::commands_received.cbegin();
    for (size_t j = 0; j < sfr::rockblock::normal_report_command_max; j++) {
        if (current_command != sfr::rockblock::commands_received.cend()) {
            sfr::rockblock::normal_report.push_back(*current_command);
            std::advance(current_command, 1);
            commands++;
        } else {
            sfr::rockblock::normal_report.push_back(0);
        }
    }
    sfr::rockblock::normal_report_command_curr = commands;
}

uint8_t NormalReportMonitor::serialize(SensorReading *valueObj)
{
    float value;
    valueObj->get_value(&value);
    return serialize(value, valueObj->get_min(), valueObj->get_max());
}

uint8_t NormalReportMonitor::serialize(SFRField valueObj)
{
    return serialize(valueObj.getFieldValue(), valueObj.getFieldMin(), valueObj.getFieldMax());
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
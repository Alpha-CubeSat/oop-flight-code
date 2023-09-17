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
        constants::rockblock::start_of_normal_downlink,

        // SFR fields
        serialize(0x1905), // sfr::burnwire::burn_time
        serialize(0x1906), // sfr::burnwire::armed_time
        serialize(0x2112), // sfr::rockblock::downlink_period
        sfr::eeprom::boot_counter,
        serialize(0x2505), // sfr::acs::Id_index
        serialize(0x2506), // sfr::acs::Kd_index
        serialize(0x2507), // sfr::acs::Kp_index
        serialize(0x2508), // sfr::acs::c_index
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

    // push the most recent 16 mission modes switches, packed into 5 bits
    int k = 0;
    bool packed_commands[80] = {false};
    auto hist_mode = sfr::mission::mode_history.cbegin();
    while (hist_mode != sfr::mission::mode_history.cend() && k <= sfr::mission::mission_mode_hist_length) {
        for (int i = 4; i >= 0; i--) {
            packed_commands[k * 5 + (4 - i)] = (*hist_mode >> i) & 1;
        }
        std::advance(hist_mode, 1);
        k++;
    }

    // read bool array
    for (int m = 0; m <= 10; m++) {
        uint8_t packed_mode = 0;
        for (int n = 0; n < 8; n++) {
            packed_mode += packed_commands[m * 8 + n] << n;
        }
        sfr::rockblock::normal_report.push_back(packed_mode);
    }

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

uint8_t NormalReportMonitor::serialize(int opcode)
{
    SFRInterface *valueObj = SFRInterface::opcode_lookup[opcode];
    float value = 0;
    int data_type = valueObj->getDataType();
    if (data_type == 4)
        value = (uint32_t)valueObj->getFieldValue();
    else if (data_type == 3)
        value = (uint16_t)valueObj->getFieldValue();
    else if (data_type == 2)
        value = (uint8_t)valueObj->getFieldValue();
    else if (data_type == 1)
        value = (bool)valueObj->getFieldValue();

    return serialize(value, valueObj->getMin(), valueObj->getMax());
}

uint8_t NormalReportMonitor::serialize(float value, float min, float max)
{
    if ((max - min) == 0) {
        return 0;
    }
    return round(map(value, min, max, 0, 255));
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
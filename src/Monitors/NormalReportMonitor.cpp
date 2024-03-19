#include "NormalReportMonitor.hpp"
#include "Faults.hpp"
#include "sfr.hpp"

NormalReportMonitor::NormalReportMonitor()
{
}

void NormalReportMonitor::execute()
{

    bool sfr_packed_bools[] = {
        sfr::photoresistor::covered, sfr::mission::possible_uncovered, sfr::camera::powered,
        sfr::mission::deployed, sfr::rockblock::waiting_command,
        sfr::temperature::in_sun, sfr::current::in_sun, sfr::button::pressed};

    bool eeprom_bools[] = {sfr::eeprom::boot_restarted, sfr::eeprom::error_mode, sfr::eeprom::light_switch, sfr::eeprom::sfr_save_completed, 0, 0, 0, 0};

    std::vector<uint8_t> report_contents{
        constants::rockblock::normal_report_flag,

        // SFR fields
        serialize(0x1802), // sfr::mission::boot_time_mins
        serialize(0x1905), // sfr::burnwire::burn_time
        serialize(0x1906), // sfr::burnwire::armed_time
        serialize(0x2110), // sfr::rockblock::lp_downlink_period
        serialize(0x2111), // sfr::rockblock::transmit_downlink_period
        serialize(0x2501), // sfr::acs::mode
        serialize(0x2506), // sfr::acs::Id_index
        serialize(0x2507), // sfr::acs::Kd_index
        serialize(0x2508), // sfr::acs::Kp_index
        serialize(0x2509), // sfr::acs::c_index
        serialize(0x2805), // sfr::eeprom::boot_counter
        serialize(0x2806), // sfr::eeprom::dynamic_data_addr
        serialize(0x2807), // sfr::eeprom::sfr_data_addr
        serialize(0x2808), // sfr::eeprom::time_alive
        serialize(0x2809), // sfr::eeprom::dynamic_data_age
        serialize(0x2810), // sfr::eeprom::sfr_data_age
        serialize(0x2505), // sfr::acs::on_time
        serialize(0x2112), // sfr::rockblock::on_time
        serialize(sfr_packed_bools),

        // Sensor readings
        serialize(sfr::photoresistor::light_val_average_standby),
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
        (uint8_t)((fault_groups::hardware_faults::light_val->serialize() << 4) + fault_groups::hardware_faults::button->serialize()),
        (uint8_t)((fault_groups::power_faults::solar_current_average->serialize() << 4) + serialize(eeprom_bools))};

    sfr::rockblock::normal_report.clear();

    for (size_t i = 0; i < report_contents.size(); i++) {
        sfr::rockblock::normal_report.push_back(report_contents[i]);
    }

    // push the most recent 16 mission modes switches, packed into 5 bits
    // if less mission modes have been entered, fill with empty mission modes
    uint32_t k = 0;
    bool packed_modes[80] = {false};
    auto hist_mode = sfr::mission::mode_history.cbegin();
    while (hist_mode != sfr::mission::mode_history.cend() && k <= constants::rockblock::mission_mode_hist_length) {
        for (int i = 4; i >= 0; i--) {
            packed_modes[k * 5 + (4 - i)] = (*hist_mode >> i) & 1;
        }
        std::advance(hist_mode, 1);
        k++;
    }

    // read bool array
    for (int m = 0; m < 10; m++) {
        uint8_t packed_mode = 0;
        for (int n = 0; n < 8; n++) {
            packed_mode += packed_modes[m * 8 + n] << (7 - n);
        }
        sfr::rockblock::normal_report.push_back(packed_mode);
    }

    // push the most recent 9 command opcodes, starting with most recent
    // if less commands have been recieved, fill with empty opcodes
    size_t commands = 0;
    auto current_command = sfr::rockblock::commands_received.cbegin();
    for (size_t j = 0; j < constants::rockblock::normal_report_command_max; j++) {
        if (current_command != sfr::rockblock::commands_received.cend()) {
            // each opcode is two bytes
            sfr::rockblock::normal_report.push_back(*current_command >> 8);
            sfr::rockblock::normal_report.push_back(*current_command & 0xff);
            std::advance(current_command, 1);
            commands++;
        } else {
            sfr::rockblock::normal_report.push_back(0);
            sfr::rockblock::normal_report.push_back(0);
        }
    }
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
    return serialize(valueObj->getFieldValue(), valueObj->getMin(), valueObj->getMax());
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

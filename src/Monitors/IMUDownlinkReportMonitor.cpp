#include "IMUDownlinkReportMonitor.hpp"
unsigned int offset_copy;
IMUDownlinkReportMonitor::IMUDownlinkReportMonitor(unsigned int offset) : TimedControlTask<void>(offset)
{
    offset_copy = offset;
}

void IMUDownlinkReportMonitor::execute()
{
    IMUDownlink imu_dlink = IMUDownlink(offset_copy);
    imu_dlink.execute();

    // Get a requested fragment
    if (sfr::imu::report_downlinked == true && sfr::imu::fragment_requested == true) {
#ifdef VERBOSE
        Serial.println("Report monitor started");
        Serial.println("Current fragment: " + String(sfr::imu::fragment_number));
#endif
        create_imu_downlink_report(sfr::imu::fragment_number_requested);
        sfr::imu::fragment_requested = false;
        sfr::imu::fragment_number++;
    }

    if (sfr::imu::fragment_number == sfr::rockblock::imu_max_fragments) {
        sfr::imu::imu_dlink_report_ready = false;
        sfr::imu::fragment_number = 0;
    }
}

void IMUDownlinkReportMonitor::create_imu_downlink_report(int fragment_number)
{
    String filename = "";
    if (fragment_number < 10) {
        filename += "0";
    }
    filename += String(fragment_number) + "_IMUDlink" + ".txt";

    sfr::rockblock::imu_downlink_report[0] = 88; //?????????

    // get each byte of fragment number
    std::vector<unsigned char> fragment(constants::imu::bytes_allocated_fragment);
    for (size_t i = 0; i < constants::imu::bytes_allocated_fragment; i++) {
        fragment[3 - i] = (fragment_number >> (i * 8));
    }

    // add fragment number to imu downlink report
    size_t i = 0;
    int a = constants::imu::bytes_allocated_serial_opcode;
    while (i < constants::imu::bytes_allocated_fragment) {
        sfr::rockblock::imu_downlink_report[a] = fragment[i];
        a = a + 1;
        i = i + 1;
    }

    // add actual image content to imu downlink report
    int buffer_size = sfr::imu::imu_dlink_magid_buffer.size();
    for (int idx = 0; idx < buffer_size; ++idx) {
        // get the magid value from buffer and write to the imu downlink report
        uint8_t magid_value = (uint8_t)sfr::imu::imu_dlink_magid_buffer.back();
        sfr::imu::imu_dlink_magid_buffer.pop_back();
        sfr::rockblock::imu_downlink_report[a++] = magid_value;

        // get the gyro values from buffer and write to the imu downlink report
        float gyro_x_value = sfr::imu::imu_dlink_gyro_x_buffer.back();
        uint8_t gyro_x = map(gyro_x_value, constants::imu::min_gyro_x, constants::imu::max_gyro_x, 0, 255);
        sfr::imu::imu_dlink_gyro_x_buffer.pop_back();
        sfr::rockblock::imu_downlink_report[a++] = gyro_x;

        float gyro_y_value = sfr::imu::imu_dlink_gyro_y_buffer.back();
        uint8_t gyro_y = map(gyro_y_value, constants::imu::min_gyro_y, constants::imu::max_gyro_y, 0, 255);
        sfr::imu::imu_dlink_gyro_y_buffer.pop_back();
        sfr::rockblock::imu_downlink_report[a++] = gyro_y;

        float gyro_z_value = sfr::imu::imu_dlink_gyro_z_buffer.back();
        uint8_t gyro_z = map(gyro_z_value, constants::imu::min_gyro_z, constants::imu::max_gyro_z, 0, 255);
        sfr::imu::imu_dlink_gyro_z_buffer.pop_back();
        sfr::rockblock::imu_downlink_report[a++] = gyro_z;
    }
    sfr::imu::imu_dlink_report_ready = true;
    sfr::imu::report_downlinked = false;
}
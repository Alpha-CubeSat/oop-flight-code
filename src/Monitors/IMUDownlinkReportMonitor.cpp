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
#endif // not sure whether we need to print this
        create_imu_downlink_report(sfr::imu::fragment_number);
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

    sfr::rockblock::imu_downlink_report[0] = 88;

    // get each byte of fragment number
    std::vector<unsigned char> fragment(constants::imu::bytes_allocated_fragment);
    fragment[0] = (fragment_number >> (0 * 8));

    // add fragment number to imu downlink report

    int a = constants::imu::bytes_allocated_serial_opcode - 1;
    size_t i = 0;
    while (i < constants::imu::bytes_allocated_fragment) {
        sfr::rockblock::imu_downlink_report[a] = fragment[i];
        a = a + 1;
        i = i + 1;
    }

    // check for the three buffer sizes
    assert(sfr::imu::imu_dlink_gyro_x_buffer.size() == sfr::imu::imu_dlink_gyro_y_buffer.size());
    assert(sfr::imu::imu_dlink_gyro_x_buffer.size() == sfr::imu::imu_dlink_gyro_z_buffer.size());

    // keep track of the byte got from the buffer
    int size_limit = 64;
    // add actual image content to imu downlink report
    int buffer_size = sfr::imu::imu_dlink_gyro_x_buffer.size();

    for (int idx = 0; idx < buffer_size; idx++) {
        // full gyro values in one report fragment
        if (size_limit - 3 < 0) {
            break;
        }

        else {
            // get the gyro values from buffer and write to the imu downlink report
            float gyro_x_value = sfr::imu::imu_dlink_gyro_x_buffer.back();
            uint8_t gyro_x = map(gyro_x_value, constants::imu::min_gyro_x, constants::imu::max_gyro_x, 0, 255);
            sfr::imu::imu_dlink_gyro_x_buffer.pop_back();
            sfr::rockblock::imu_downlink_report[a++] = gyro_x;
            size_limit--;

            float gyro_y_value = sfr::imu::imu_dlink_gyro_y_buffer.back();
            uint8_t gyro_y = map(gyro_y_value, constants::imu::min_gyro_y, constants::imu::max_gyro_y, 0, 255);
            sfr::imu::imu_dlink_gyro_y_buffer.pop_back();
            sfr::rockblock::imu_downlink_report[a++] = gyro_y;
            size_limit--;

            float gyro_z_value = sfr::imu::imu_dlink_gyro_z_buffer.back();
            uint8_t gyro_z = map(gyro_z_value, constants::imu::min_gyro_z, constants::imu::max_gyro_z, 0, 255);
            sfr::imu::imu_dlink_gyro_z_buffer.pop_back();
            sfr::rockblock::imu_downlink_report[a++] = gyro_z;
            size_limit--;
        }
        // // end marker
        // if (idx == buffer_size - 1) {
        //     sfr::rockblock::imu_downlink_report[a++] = 0xF;
        //     sfr::rockblock::imu_downlink_report[a++] = 0xF;
        // }
    }
    sfr::imu::imu_dlink_report_ready = true;
    sfr::imu::report_downlinked = false;
}
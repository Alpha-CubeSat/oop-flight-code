#include "IMUDownlinkReportMonitor.hpp"

IMUDownlinkReportMonitor::IMUDownlinkReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void IMUDownlinkReportMonitor::execute()
{
    // Fragment value has reaches a max and can no longer create a report
    if (fragment_number >= sfr::imu::max_fragments) {
        sfr::imu::report_ready = false;
    }

    // Create an IMU report when ever the report is ready
    if (fragment_number < sfr::imu::max_fragments && sfr::rockblock::imu_report.empty() && sfr::imu::report_written) {
        create_imu_downlink_report(fragment_number);
        fragment_number++;
    }

    if (sfr::imu::fragment_requested && !sfr::imu::report_ready) {
        create_imu_downlink_report_from_SD(sfr::imu::fragment_number_requested);
    }
}

void IMUDownlinkReportMonitor::create_imu_downlink_report(uint8_t fragment_number)
{
    // Set report is not ready if imu_dlink is empty and all fragments have been downlinked
    if (sfr::imu::imu_dlink.size() == 0 && sfr::rockblock::imu_report.size() == 0) {
        sfr::imu::report_ready = false;
        return;
    }

    // Set up fragment save to SD card
    String filename = "imu_frag_" + String(fragment_number) + ".txt";
    File txtFile = SD.open(filename.c_str(), FILE_WRITE);

    // pushed imu report id
    sfr::rockblock::imu_report.push_back(24);
    txtFile.print(24, HEX);

    // Push fragment number to the report
    sfr::rockblock::imu_report.push_back(fragment_number);
    txtFile.print(24, HEX);

    // Add values to the report and delete from buffer.
    for (int i = 0; i < pop_size; i++) {
        uint8_t data = sfr::imu::imu_dlink.back();
        sfr::rockblock::imu_report.push_back(data);
        txtFile.print(data, HEX);
        sfr::imu::imu_dlink.pop_back();
    }
    // Push end flag at the end of the report
    sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag1);
    sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag2);

    // place the endflag at the end of the message
    if (fragment_number >= (int)(constants::imu_downlink::downlink_FIFO_byte_length / pop_size - 1)) {
        sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag1);
        sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag2);
        txtFile.print(constants::imu_downlink::imu_report_endflag1, HEX);
        txtFile.print(constants::imu_downlink::imu_report_endflag2, HEX);
    }

    txtFile.close();
    // write_imu_report_to_SD(fragment_number);
    // for the next downlink cycle
    sfr::imu::report_ready = true;
}

void IMUDownlinkReportMonitor::create_imu_downlink_report_from_SD(uint8_t fragment_number)
{
    // open image file and read it for specified image/fragment
    String filename = "imu_frag_" + String(fragment_number) + ".txt";
    File txtFile = SD.open(filename.c_str(), FILE_READ);

    // parse hex stored as chars into actual hex
    uint8_t tempbuffer[constants::imu::max_gyro_imu_report_size];
    uint8_t parsedbuffer[constants::camera::content_length];
    for (size_t i = 0; i < sizeof(tempbuffer); i++) {
        tempbuffer[i] = txtFile.read();
    }

    txtFile.read(tempbuffer, constants::camera::content_length);
    int x = 0;
    for (size_t i = 0; i < sizeof(tempbuffer); i++) {
        int byte_0;
        int byte_1;
        if (tempbuffer[i] <= 90 && tempbuffer[i] >= 65) {
            byte_0 = tempbuffer[i] - 55;
        } else {
            byte_0 = tempbuffer[i] - 48;
        }
        if (tempbuffer[i + 1] <= 90 && tempbuffer[i + 1] >= 65) {
            byte_1 = tempbuffer[i + 1] - 55;
        } else {
            byte_1 = tempbuffer[i + 1] - 48;
        }
        parsedbuffer[x] = byte_1 + (byte_0 * 16);
        x++;
        i++;
    }
    imgFile.close();

    sfr::rockblock::camera_report.push_back(42);
    // get each byte of serial number and add to camera report
    sfr::rockblock::camera_report.push_back(serial_number);

    // get each byte of fragment number
    std::vector<unsigned char> fragment(constants::camera::bytes_allocated_fragment);
    for (size_t i = 0; i < constants::camera::bytes_allocated_fragment; i++) {
        fragment[3 - i] = (fragment_number >> (i * 8));
        sfr::rockblock::camera_report.push_back(fragment[i]);
    }

    // add actual image content to camera report
    for (int i = 0; i < constants::camera::content_length; i++) {
        sfr::rockblock::camera_report.push_back(parsedbuffer[i]);
    }
#ifdef E2E_TESTNG
    Serial.println("Camera report ready");
#endif
    sfr::camera::report_ready = true;
}

// SD write is already done as report is being made, but can be factored out in this function if that is safer
void IMUDownlinkReportMonitor::write_imu_report_to_SD(uint8_t fragment_number)
{
    String filename = "imu_frag_" + String(fragment_number) + ".txt";
    File txtFile = SD.open(filename.c_str(), FILE_WRITE);

    for (uint8_t &data : sfr::rockblock::imu_report) {
        txtFile.print(data, HEX);
    }

    txtFile.close();
}
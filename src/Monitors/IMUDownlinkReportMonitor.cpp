#include "IMUDownlinkReportMonitor.hpp"

IMUDownlinkReportMonitor::IMUDownlinkReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void IMUDownlinkReportMonitor::execute()
{
    // We have reached the maximum capacity we could have for the fragments
    if (fragment_number >= sfr::imu::max_fragments) {
        sfr::imu::report_ready = false;
    }

    // Create an IMU report when ever the report is ready
    if (fragment_number < sfr::imu::max_fragments && sfr::rockblock::imu_report.empty() && sfr::imu::report_written) {
        create_imu_downlink_report(fragment_number);
        fragment_number++;
    }

    // A fragment request has been made, and there is no report currently queued
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

    // Push imu report id
    sfr::rockblock::imu_report.push_back(24);

    // Push fragment number to the report
    sfr::rockblock::imu_report.push_back(fragment_number);

    // set pop_size to the minimum of the content_length limit,
    // the number of readings ready in the queue, and the max report size
    int pop_size = min(min(constants::imu::max_gyro_imu_report_size,
                           constants::rockblock::content_length),
                       sfr::imu::imu_dlink.size());

    // Set up fragment save to SD card
    String filename = "imu_frag_" + String(fragment_number) + ".txt";
    File txtFile = SD.open(filename.c_str(), FILE_WRITE);

    // Add actual gyro content to imu report
    for (int i = 0; i < pop_size; i++) {
        uint8_t data = sfr::imu::imu_dlink.back();
        sfr::rockblock::imu_report.push_back(data);
        txtFile.print(data, HEX);
        sfr::imu::imu_dlink.pop_back();
    }

    // Push end flag at the end of the report
    sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag1);
    sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag2);

    txtFile.close();

    // Place the endflag at the end of the message
    sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag1);
    sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag2);

    // For the next downlink cycle
    sfr::imu::report_ready = true;
}

void IMUDownlinkReportMonitor::create_imu_downlink_report_from_SD(uint8_t fragment_number)
{
    // Open image file and read it for specified image/fragment
    String filename = "imu_frag_" + String(fragment_number) + ".txt";
    File txtFile = SD.open(filename.c_str(), FILE_READ);

    // Parse hex stored as chars into actual hex
    uint8_t tempbuffer[constants::imu::max_gyro_imu_report_size];
    uint8_t parsedbuffer[constants::imu::max_gyro_imu_report_size];

    txtFile.read(tempbuffer, constants::imu::max_gyro_imu_report_size);

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

    txtFile.close();

    sfr::rockblock::imu_report.push_back(24);
    sfr::rockblock::imu_report.push_back(fragment_number);

    // Add fragment data to imu report
    for (int i = 0; i < constants::camera::content_length; i++) {
        sfr::rockblock::imu_report.push_back(parsedbuffer[i]);
    }

    sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag1);
    sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag2);

#ifdef E2E_TESTNG
    Serial.println("IMU report ready");
#endif

    sfr::imu::report_ready = true;
    sfr::imu::fragment_requested = false;
}
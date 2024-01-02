#include "IMUDownlinkReportMonitor.hpp"

IMUDownlinkReportMonitor::IMUDownlinkReportMonitor() {}

void IMUDownlinkReportMonitor::execute()
{
    // Fragment value has reaches the max count and there are no more IMU reports to be sent
    if (fragment_number >= sfr::imu::max_fragments) {
        sfr::imu::report_ready = false;
    }

    // A fragment request has been made, and there is no report currently queued
    if (sfr::imu::fragment_requested && sfr::rockblock::imu_report.empty()) {
#ifdef VERBOSE
        Serial.println("Requested IMU fragment " + String(sfr::imu::fragment_number_requested));
#endif
        // Will not do anything if the requested fragment's text file does not exist
        create_imu_downlink_report_from_SD(sfr::imu::fragment_number_requested);
    }

    // Create an IMU report when ever the report is ready
    if (fragment_number < sfr::imu::max_fragments && sfr::rockblock::imu_report.empty() && sfr::imu::report_written) {
#ifdef VERBOSE
        Serial.println("Writing IMU fragment " + String(fragment_number));
#endif
        create_imu_downlink_report(fragment_number);
        fragment_number++;
    }
}

void IMUDownlinkReportMonitor::create_imu_downlink_report(uint8_t fragment_number)
{
    // Set report is not ready if imu_dlink is empty and all fragments have been downlinked
    if (sfr::imu::imu_dlink.size() == 0 && sfr::rockblock::imu_report.size() == 0) {
        sfr::imu::report_ready = false;
        sfr::rockblock::imu_report.clear();
        return;
    }

    // Sets the amount of values that go into the report.
    int pop_size = min(constants::imu::max_gyro_imu_report_size, sfr::imu::imu_dlink.size());

    // Push report ID
    sfr::rockblock::imu_report.push_back(constants::rockblock::imu_report_flag);

    // Push fragment number to the report
    sfr::rockblock::imu_report.push_back(fragment_number);

    // Add values to the report and delete from buffer.
    for (int i = 0; i < pop_size; i++) {
        sfr::rockblock::imu_report.push_back(sfr::imu::imu_dlink.back());
        sfr::imu::imu_dlink.pop_back();
    }
    // Push end flag at the end of the report
    sfr::rockblock::imu_report.push_back(constants::rockblock::imu_report_endflag1);
    sfr::rockblock::imu_report.push_back(constants::rockblock::imu_report_endflag2);

    // For the next downlink cycle
    sfr::imu::report_ready = true;
}

void IMUDownlinkReportMonitor::create_imu_downlink_report_from_SD(uint8_t fragment_number)
{
    // Open image file and read it for specified image/fragment
    String filename = "imu_frag_" + String(fragment_number) + ".txt";
    File txtFile = SD.open(filename.c_str(), FILE_READ);

    if (txtFile) {
        // Text file exists on SD card

        // Parse hex stored as chars into actual hex
        uint8_t tempbuffer[constants::imu::max_gyro_imu_report_size * 2];
        uint8_t parsedbuffer[constants::imu::max_gyro_imu_report_size];

        for (size_t i = 0; i < sizeof(tempbuffer); i++) {
            tempbuffer[i] = txtFile.read();
        }

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

        sfr::rockblock::imu_report.push_back(constants::rockblock::imu_report_flag);
        sfr::rockblock::imu_report.push_back(fragment_number);

        // Add fragment data to imu report
        for (int i = 0; i < constants::camera::content_length; i++) {
            sfr::rockblock::imu_report.push_back(parsedbuffer[i]);
        }

        sfr::rockblock::imu_report.push_back(constants::rockblock::imu_report_endflag1);
        sfr::rockblock::imu_report.push_back(constants::rockblock::imu_report_endflag2);

        sfr::imu::report_ready = true;
        sfr::imu::fragment_requested = false;
    }
}
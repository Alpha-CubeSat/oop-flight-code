#include "IMUDownlinkReportMonitor.hpp"
IMUDownlinkReportMonitor::IMUDownlinkReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void IMUDownlinkReportMonitor::execute()
{
    // Serial.println("imu_dlink size is " + String(sfr::imu::imu_dlink.size()));
    // Serial.println("Current fragment: " + String(sfr::imu::fragment_number));
    // Serial.println("sfr::imu::report_downlinked: " + String(sfr::imu::report_downlinked));
    // Serial.println("sfr::imu::report_written: " + String(sfr::imu::report_written));
    if (sfr::imu::report_downlinked == true && sfr::imu::report_written) {
        // Serial.println("!!!!!!I will create IMU downlink report");
        // #ifdef VERBOSE_IMUDs
        //         Serial.println("Report monitor started");
        //         Serial.println("Current fragment: " + String(sfr::imu::fragment_number));
        // #endif
        if (sfr::imu::full_report_written == true || sfr::imu::fragment_number == 0) {
            sfr::imu::full_report_written = false;
        }
        create_imu_downlink_report(sfr::imu::fragment_number);
        if (sfr::imu::imu_dlink.size() == 0) {
            sfr::imu::report_ready = false;
            // TODO
            // add_possible_command();
        } else {
            sfr::imu::fragment_number++;
        }
    }

    if (sfr::imu::fragment_number >= sfr::imu::max_fragments) {
        sfr::imu::report_ready = false;
        sfr::imu::fragment_number = 0;
    }
}

void IMUDownlinkReportMonitor::create_imu_downlink_report(int fragment_number)
{

    sfr::rockblock::imu_report.push_back(24);

    sfr::rockblock::imu_report.push_back(sfr::imu::fragment_number);
    int pop_size = sfr::imu::content_length;
    if (sfr::imu::content_length > sfr::imu::imu_dlink.size()) {
        pop_size = sfr::imu::imu_dlink.size();
    }

    // add actual gyro content to imu report
    int z = 0;
    while (z < pop_size && z < 66) {
        sfr::rockblock::imu_report.push_back(sfr::imu::imu_dlink.back());
        sfr::imu::imu_dlink.pop_back();
        z = z + 1;
    }
    sfr::imu::report_ready = true;
    sfr::imu::report_downlinked = false;
}
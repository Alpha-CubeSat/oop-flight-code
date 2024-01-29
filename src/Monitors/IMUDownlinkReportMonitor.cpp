#include "IMUDownlinkReportMonitor.hpp"

IMUDownlinkReportMonitor::IMUDownlinkReportMonitor() {}

void IMUDownlinkReportMonitor::execute()
{
    // Fragment value has reaches the max count and there are no more IMU reports to be sent
    if (fragment_number >= sfr::imu::max_fragments) {
        sfr::imu::report_ready = false;
    }

    // Create an IMU report when ever the report is ready
    if (fragment_number < sfr::imu::max_fragments && sfr::rockblock::imu_report.empty() && sfr::imu::dlink_written) {
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

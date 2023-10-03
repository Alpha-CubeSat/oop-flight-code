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
}

// Creates IMU report given a fragment number
void IMUDownlinkReportMonitor::create_imu_downlink_report(int fragment_number)
{
    // Set report is not ready if imu_dlink is empty and all fragments have been downlinked
    if (sfr::imu::imu_dlink.size() == 0 && sfr::rockblock::imu_report.size() == 0) {
        sfr::imu::report_ready = false;
        return;
    }

    // Push report ID 
    sfr::rockblock::imu_report.push_back(24);

    // Push fragment number to the report
    sfr::rockblock::imu_report.push_back(fragment_number);

    // set pop_size to the minimum of the content_length limit,
    // the number of readings ready in the queue, and the max report size
    // Sets the amount of values that go into the report.
    int pop_size = min(min(constants::imu::max_gyro_imu_report_size, constants::rockblock::content_length), sfr::imu::imu_dlink.size());

    // Add values to the report and delete from buffer.
    for (int i = 0; i < pop_size; i++) {
        sfr::rockblock::imu_report.push_back(sfr::imu::imu_dlink.back());
        sfr::imu::imu_dlink.pop_back();
    }

    // Push end flag at the end of the report
    if (fragment_number >= (int)(constants::imu_downlink::max_IMU_report_length / pop_size - 1)) {
        sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag1);
        sfr::rockblock::imu_report.push_back(constants::imu_downlink::imu_report_endflag2);
    }

    // Set report is ready for the next downlink 
    sfr::imu::report_ready = true;
}
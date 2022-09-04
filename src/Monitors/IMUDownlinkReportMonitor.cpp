#include "IMUDownlinkReportMonitor.hpp"

IMUDownlinkReportMonitor::IMUDownlinkReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void IMUDownlinkReportMonitor::execute()
{
    if (report_downlinked == true && report_written) {
        if (full_report_written == true || fragment_number == 0) {
            full_report_written = false;
        }
        create_imu_downlink_report(fragment_number);
        // imu_downlink buffer is empty and all fragments have been downlinked
        if (sfr::imu::imu_dlink.size() == 0) {
            report_ready = false;
            // TODO
            // add_possible_command();
        } else {
            fragment_number++;
        }
    }
    // we have reached the maximum capacity we could have for the fragments
    if (fragment_number >= sfr::imu::max_fragments) {
        report_ready = false;
        fragment_number = 0;
    }
}

void IMUDownlinkReportMonitor::create_imu_downlink_report(int fragment_number)
{

    // pushed imu report id
    sfr::rockblock::imu_report.push_back(24);

    // pushed imu fragment number
    sfr::rockblock::imu_report.push_back(fragment_number);
    int pop_size = constants::rockblock::content_length;
    if (constants::rockblock::content_length > sfr::imu::imu_dlink.size()) {
        pop_size = sfr::imu::imu_dlink.size();
    }

    // add actual gyro content to imu report
    for (int i = 0; i < min(pop_size, constants::imu::max_gyro_imu_report_size); i++) {
        sfr::rockblock::imu_report.push_back(sfr::imu::imu_dlink.back());
        sfr::imu::imu_dlink.pop_back();
    }
    // for the next downlink cycle
    report_ready = true;
    report_downlinked = false;
}
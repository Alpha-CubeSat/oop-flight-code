#include "IMUDownlinkReportMonitor.hpp"

IMUDownlinkReportMonitor::IMUDownlinkReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void IMUDownlinkReportMonitor::execute()
{
    // we have reached the maximum capacity we could have for the fragments,
    if (fragment_number >= sfr::imu::max_fragments) {
        sfr::imu::report_ready = false;
    }

    if (fragment_number < sfr::imu::max_fragments && sfr::rockblock::imu_report.empty() && sfr::imu::report_written) {
        create_imu_downlink_report(fragment_number);
        fragment_number++;
    }
}

void IMUDownlinkReportMonitor::create_imu_downlink_report(int fragment_number)
{

    // imu_dlink empty and all fragments have been downlinked
    if (sfr::imu::imu_dlink.size() == 0 && sfr::rockblock::imu_report.size() == 0) {
        sfr::imu::report_ready = false;
        return;
    }

    // pushed imu report id
    sfr::rockblock::imu_report.push_back(24);

    // pushed imu fragment number
    sfr::rockblock::imu_report.push_back(fragment_number);

    // set pop_size to the minimum of the content_length limit,
    // the number of readings ready in the queue, and the max report size
    int pop_size = min(min(constants::imu::max_gyro_imu_report_size,
                           constants::rockblock::content_length),
                       sfr::imu::imu_dlink.size());

    // add actual gyro content to imu report
    for (int i = 0; i < pop_size; i++) {
        sfr::rockblock::imu_report.push_back(sfr::imu::imu_dlink.back());
        sfr::imu::imu_dlink.pop_back();
    }

    // for the next downlink cycle
    sfr::imu::report_ready = true;
}
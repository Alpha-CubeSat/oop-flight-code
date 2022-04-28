#include "IMUDownlinkReportMonitor.hpp"
IMUDownlinkReportMonitor::IMUDownlinkReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void IMUDownlinkReportMonitor::execute()
{
    if (sfr::imu::report_downlinked == true && sfr::imu::report_written) {
#ifdef VERBOSE_IMUD 
        Serial.println("Report monitor started");
        Serial.println("Current fragment: " + String(sfr::imu::fragment_number));
#endif
        if (sfr::imu::full_report_written == true || sfr::imu::fragment_number == 0) {
            sfr::imu::full_report_written = false;
        }
       create_imu_downlink_report(sfr::imu::fragment_number);
        if (sfr::imu::imu_dlink.size() == 0) {
            // TODO
            //add_possible_command();
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
    // open image file and read it for specified image/fragment

    // get from buffer and put in parsed buffer
    sfr::rockblock::imu_report.push_back(88);

    sfr::rockblock::imu_report.push_back(sfr::imu::fragment_number);
 
    // add actual image content to camera report
    int z = 0;
    while (z < sfr::imu::content_length) {
        sfr::rockblock::imu_report.push_back(sfr::imu::imu_dlink.back());
        sfr::imu::imu_dlink.pop_back();
        z = z + 1;
    }
    sfr::imu::report_ready = true;
    sfr::imu::report_downlinked = false;
}
#include "RockblockReportMonitor.hpp"

RockblockReportMonitor::RockblockReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void RockblockReportMonitor::execute()
{
    rockblock_mode_type mode = sfr::rockblock::mode;

    switch (mode) {
    case rockblock_mode_type::standby:
        schedule_report();
        break;
    }

    switch (sfr::rockblock::downlink_report_type) {
    case report_type::camera_report:
        sfr::rockblock::downlink_report.clear();
        for (auto &data : sfr::rockblock::camera_report) {
            sfr::rockblock::downlink_report.push_back(data);
        }
        break;

    case report_type::normal_report:
        sfr::rockblock::downlink_report.clear();
        for (auto &data : sfr::rockblock::normal_report) {
            sfr::rockblock::downlink_report.push_back(data);
        }
        break;

    case report_type::imu_report:
        sfr::rockblock::downlink_report.clear();
        // for (int i = 0; i < constants::rockblock::packet_size; i++) {
        //     sfr::rockblock::downlink_report.push_back(sfr::rockblock::imu_report[i]);
        // }
        for (auto &data : sfr::rockblock::imu_report) {
#ifdef VERBOSE_IMUP
            Serial.print("data from imu_downlink_report: " + String(data));
#endif
            sfr::rockblock::downlink_report.push_back(data);
        }
        break;
    }
}

void RockblockReportMonitor::schedule_report()
{

    // Schedule IMU Report: Highest Priority
    if (sfr::imu::imu_dlink_report_ready) {
        sfr::rockblock::downlink_report_type = report_type::imu_report;
        sfr::rockblock::rockblock_ready_status = true;
        return;
    }

    // Schedule Normal Report: Second Highest Priority
    if (millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period) {
        sfr::rockblock::downlink_report_type = report_type::normal_report;
        sfr::rockblock::rockblock_ready_status = true;
        return;
    }

    // Schedule Camera Report: Third Highest Priority
    if (sfr::camera::report_ready == true) {
        sfr::rockblock::downlink_report_type = report_type::camera_report;
        sfr::rockblock::rockblock_ready_status = true;
        return;
    }

    sfr::rockblock::rockblock_ready_status = false;
}
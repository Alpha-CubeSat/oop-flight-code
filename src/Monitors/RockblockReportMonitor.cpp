#include "RockblockReportMonitor.hpp"

RockblockReportMonitor::RockblockReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void RockblockReportMonitor::execute()
{
    rockblock_mode_type mode = sfr::rockblock::mode;

    switch (mode) {
    case rockblock_mode_type::standby:
        standby_update_downlink_report_type();
        break;
    }

    switch (sfr::rockblock::downlink_report_type) {
    case report_type::camera_report:
        for (int i = 0; i < constants::rockblock::packet_size; i++) {
            sfr::rockblock::downlink_report[i] = sfr::rockblock::camera_report[i];
        }
        break;

    case report_type::normal_report:
        for (int i = 0; i < constants::rockblock::packet_size; i++) {
            sfr::rockblock::downlink_report[i] = sfr::rockblock::normal_report[i];
        }
        break;

    case report_type::imu_report:
        for (int i = 0; i < constants::imu::downlink_report_size; i++) {
            sfr::rockblock::downlink_report[i] = sfr::rockblock::imu_report[i];
        }

        for (int i = constants::imu::downlink_report_size; i < i < constants::rockblock::packet_size; i++) {
            sfr::rockblock::downlink_report[i] = 0;
        }
        break;
    }
}

void RockblockReportMonitor::standby_update_downlink_report_type()
{
    if (millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period) {
        if (sfr::rockblock::downlink_period == sfr::rockblock::camera_downlink_period && sfr::camera::report_ready == true) {
            if (sfr::rockblock::last_downlink_report_type == report_type::imu_report) {
                sfr::rockblock::downlink_report_type = report_type::camera_report;
            }
        }

        if (sfr::rockblock::last_downlink_report_type == report_type::camera_report) {
            sfr::rockblock::downlink_report_type = report_type::normal_report;
        }

        if (sfr::rockblock::last_downlink_report_type == report_type::normal_report && sfr::imu::imu_dlink_report_ready) {
            sfr::rockblock::downlink_report_type = report_type::imu_report;
        }

        sfr::rockblock::rockblock_ready_status = true;
        return;
    }

    if ((millis() - sfr::rockblock::last_downlink >= sfr::rockblock::camera_downlink_period) && sfr::camera::report_ready == true) {
        sfr::rockblock::downlink_report_type = report_type::camera_report;
        sfr::rockblock::rockblock_ready_status = true;
        return;
    }

    sfr::rockblock::rockblock_ready_status = false;
}
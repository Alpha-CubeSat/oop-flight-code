#include "RockblockReportMonitor.hpp"
#include "sfr.hpp"

RockblockReportMonitor::RockblockReportMonitor()
{
}

void RockblockReportMonitor::execute()
{
    switch (static_cast<rockblock_mode_type>(sfr::rockblock::mode.get())) {
    // Report scheduling is only performed in `rockblock_mode_type::standby` mode
    case rockblock_mode_type::standby:
        schedule_report();
        break;
    default:
        break;
    }

    switch (static_cast<report_type>(sfr::rockblock::downlink_report_type.get())) {
    case report_type::camera_report:
        sfr::rockblock::downlink_report.clear();
        for (auto &data : sfr::rockblock::camera_report) {
            sfr::rockblock::downlink_report.push_back(data);
        }
        return;

    case report_type::normal_report:
        sfr::rockblock::downlink_report.clear();
        for (auto &data : sfr::rockblock::normal_report) {
            sfr::rockblock::downlink_report.push_back(data);
        }
        return;

    case report_type::imu_report:
        sfr::rockblock::downlink_report.clear();
        for (auto &data : sfr::rockblock::imu_report) {
            sfr::rockblock::downlink_report.push_back(data);
        }
        return;
    }
}

void RockblockReportMonitor::switch_report_type_to(report_type downlink_report_type)
{
    sfr::rockblock::downlink_report_type = (uint16_t)downlink_report_type;
    sfr::rockblock::ready_status = true;
}

void RockblockReportMonitor::schedule_report()
{

    // Check if in a low-power mode; if so, only enable normal report downlink
    if (sfr::mission::current_mode->get_type() == mode_type::LP) {
        if (millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period) {
            sfr::rockblock::downlink_report_type = (uint16_t)report_type::normal_report;
            sfr::rockblock::ready_status = true;
            return;
        }

        sfr::rockblock::ready_status = false;
        return;
    }

    // Not in low-power mode; report type cycle order: normal report, IMU report, camera report
    switch (static_cast<report_type>(sfr::rockblock::downlink_report_type.get())) {
    case report_type::normal_report:
        if (sfr::imu::report_ready) {
            switch_report_type_to(report_type::imu_report);
            return;
        }

        if (sfr::camera::report_ready) {
            switch_report_type_to(report_type::camera_report);
            return;
        }

        if (millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period) {
            switch_report_type_to(report_type::normal_report);
            return;
        }

        // None of the reports are ready
        sfr::rockblock::ready_status = false;
        return;
    case report_type::imu_report:
        if (sfr::camera::report_ready) {
            switch_report_type_to(report_type::camera_report);
            return;
        }

        if (millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period) {
            switch_report_type_to(report_type::normal_report);
            return;
        }

        if (sfr::imu::report_ready) {
            switch_report_type_to(report_type::imu_report);
            return;
        }

        // None of the reports are ready
        sfr::rockblock::ready_status = false;
        return;
    case report_type::camera_report:
        if (millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period) {
            switch_report_type_to(report_type::normal_report);
            return;
        }

        if (sfr::imu::report_ready) {
            switch_report_type_to(report_type::imu_report);
            return;
        }

        if (sfr::camera::report_ready) {
            switch_report_type_to(report_type::camera_report);
            return;
        }

        // None of the reports are ready
        sfr::rockblock::ready_status = false;
        return;
    }
}
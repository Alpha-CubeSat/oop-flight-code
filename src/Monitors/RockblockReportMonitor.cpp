#include "RockblockReportMonitor.hpp"
#include "sfr.hpp"

RockblockReportMonitor::RockblockReportMonitor()
{
}

void RockblockReportMonitor::execute()
{
    if(sfr::rockblock::mode == (uint16_t)rockblock_mode_type::standby){
        schedule_report();
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
    if (!sfr::rockblock::sleep_mode && (millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period || !attempted_downlink)) {
        attempted_downlink = true;
        // Check if in a low-power mode
        if (sfr::mission::current_mode->get_type() == mode_type::LP) {
            // In low-power mode; only enable normal report downlinks
            sfr::rockblock::downlink_report_type = (uint16_t)report_type::normal_report;
            sfr::rockblock::ready_status = true;
            return;
        } else {
            // Not in low-power mode; report type cycle order: normal report, IMU report, camera report
            switch (static_cast<report_type>(sfr::rockblock::downlink_report_type.get())) {
            case report_type::normal_report:
                if (sfr::camera::report_ready) {
                    switch_report_type_to(report_type::camera_report);
                    return;
                } else if (sfr::imu::report_ready) {
                    switch_report_type_to(report_type::imu_report);
                    return;
                }
                switch_report_type_to(report_type::normal_report);
                return;
            case report_type::camera_report:
                if (sfr::imu::report_ready) {
                    switch_report_type_to(report_type::imu_report);
                    return;
                }
                switch_report_type_to(report_type::normal_report);
                return;
            case report_type::imu_report:
                switch_report_type_to(report_type::normal_report);
                return;
            }
        }
    } else {
        sfr::rockblock::ready_status = false;
    }
}

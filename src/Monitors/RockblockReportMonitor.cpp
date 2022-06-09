#include "RockblockReportMonitor.hpp"

RockblockReportMonitor::RockblockReportMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void RockblockReportMonitor::execute()
{
    rockblock_mode_type mode = sfr::rockblock::mode;

    switch (mode) {
    case rockblock_mode_type::standby:
        schedule_report();
        break;
    default:
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
        for (auto &data : sfr::rockblock::imu_report) {
            sfr::rockblock::downlink_report.push_back(data);
        }
        break;
    }
}

bool check_in_lower_power_mode()
{
    return sfr::mission::current_mode == sfr::mission::lowPowerAliveSignal ||
           sfr::mission::current_mode == sfr::mission::lowPowerDetumbleSpin ||
           sfr::mission::current_mode == sfr::mission::lowPower ||
           sfr::mission::current_mode == sfr::mission::lowPowerDeployment ||
           sfr::mission::current_mode == sfr::mission::lowPowerArmed ||
           sfr::mission::current_mode == sfr::mission::lowPowerInSun;
}

void RockblockReportMonitor::schedule_report()
{

    // Check if in a low-power mode; if so, only allow normal report downlink
    if (check_in_lower_power_mode()) {
        if (millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period) {
            sfr::rockblock::downlink_report_type = report_type::normal_report;
            sfr::rockblock::rockblock_ready_status = true;
            return;
        }

        sfr::rockblock::rockblock_ready_status = false;
        return;
    }
}
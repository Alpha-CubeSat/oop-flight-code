#include "RockblockReportMonitor.hpp"

RockblockReportMonitor::RockblockReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void RockblockReportMonitor::execute()
{
}

void RockblockReportMonitor::update_ready_status()
{
    if (millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period) {
        if (sfr::rockblock::downlink_period == sfr::rockblock::camera_downlink_period && sfr::camera::report_ready == true) {
            if (sfr::rockblock::last_downlink_normal == false) {
                sfr::rockblock::downlink_camera = false;
            } else {
                sfr::rockblock::downlink_camera = true;
            }
        }

        return;
    }

    if ((millis() - sfr::rockblock::last_downlink >= sfr::rockblock::camera_downlink_period) && sfr::camera::report_ready == true) {
        sfr::rockblock::downlink_camera = true;
    }
}
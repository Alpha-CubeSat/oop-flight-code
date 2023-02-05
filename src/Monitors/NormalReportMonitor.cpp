#include "NormalReportMonitor.hpp"
#include "sfr.hpp"

NormalReportMonitor::NormalReportMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void NormalReportMonitor::execute()
{
    std::deque<uint8_t> empty_normal_report;
    std::swap(sfr::rockblock::normal_report, empty_normal_report);
    // TODO serialize sensor data and push to NormalReport buffer. FS-161
    int i = 0;
    auto current_command = sfr::rockblock::commands_received.cbegin();
    while (current_command != sfr::rockblock::commands_received.cend() && i < sfr::rockblock::normal_report_command_max) {
        sfr::rockblock::normal_report.push_back(*current_command);
        i++;
    }
    sfr::rockblock::normal_report_command_curr = i;
    sfr::rockblock::normal_report.push_back(constants::rockblock::end_of_normal_downlink_flag1);
    sfr::rockblock::normal_report.push_back(constants::rockblock::end_of_normal_downlink_flag2);
}
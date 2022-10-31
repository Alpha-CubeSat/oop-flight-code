#include "NormalReportMonitor.hpp"
#include "sfr.hpp"

NormalReportMonitor::NormalReportMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

std::queue<uint8_t> NormalReportMonitor::commands_received;

void NormalReportMonitor::execute()
{
    // TODO serialize sensor data and push to NormalReport buffer. FS-161
    /*int i = 0;
    while (!commands_received.empty() && i < 30) {
        sfr::rockblock::normal_report.push_back(commands_received.front());
        commands_received.pop();
        ++i;
    } // Writes opcodes to normal report; two indices constitute one opcode since each opcode is 2-byte
    std::queue<uint8_t> empty;
    std::swap(commands_received, empty); // Clear the queue after each normal report is generated
    sfr::rockblock::normal_report.push_back(constants::rockblock::end_of_normal_downlink_flag1);
    sfr::rockblock::normal_report.push_back(constants::rockblock::end_of_normal_downlink_flag2);*/
}
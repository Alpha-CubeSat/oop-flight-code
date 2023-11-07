#ifndef ROCKBLOCK_REPORT_MONITOR_HPP_
#define ROCKBLOCK_REPORT_MONITOR_HPP_

#include "sfr.hpp"

class RockblockReportMonitor
{
public:
    RockblockReportMonitor();
    void execute();

private:
    void switch_report_type_to(report_type downlink_report_type);
    void schedule_report();
    report_type last_report_type;
};

#endif
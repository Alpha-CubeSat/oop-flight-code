#ifndef ROCKBLOCK_REPORT_MONITOR_HPP_
#define ROCKBLOCK_REPORT_MONITOR_HPP_

#include "sfr.hpp"

class RockblockReportMonitor : public TimedControlTask<void>
{
public:
    RockblockReportMonitor(unsigned int offset);
    void execute();
    void standby_update_downlink_report_type();
};

#endif
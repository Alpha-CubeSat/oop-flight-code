#ifndef ROCKBLOCK_REPORT_MONITOR_HPP_
#define ROCKBLOCK_REPORT_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "sfr.hpp"

class RockblockReportMonitor : public TimedControlTask<void>
{
public:
    RockblockReportMonitor(unsigned int offset);
    void execute();

private:
    void switch_report_type_to(report_type downlink_report_type);
    void schedule_report();
};

#endif
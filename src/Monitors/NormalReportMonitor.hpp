#ifndef NORMAL_REPORT_MONITOR_HPP_
#define NORMAL_REPORT_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "Faults.hpp"
#include <queue>

class NormalReportMonitor : public TimedControlTask<void>
{
public:
    NormalReportMonitor(unsigned int offset);
    void execute();
};

#endif
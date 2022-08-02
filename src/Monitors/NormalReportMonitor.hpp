#ifndef NORMAL_REPORT_MONITOR_HPP_
#define NORMAL_REPORT_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "Faults.hpp"
#include <queue>

class NormalReportMonitor : public TimedControlTask<void>
{
public:
    static std::queue<uint8_t> commands_received; // Queue to hold commands received since last report

    NormalReportMonitor(unsigned int offset);
    void execute();
};

#endif
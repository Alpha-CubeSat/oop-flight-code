#ifndef NORMAL_REPORT_MONITOR_HPP_
#define NORMAL_REPORT_MONITOR_HPP_

#include "sfr.hpp"
#include <queue>

class NormalReportMonitor : public TimedControlTask<void>
{
private:
    std::queue<uint8_t> commands_received;

public:
    NormalReportMonitor(unsigned int offset);
    void execute();
};

#endif
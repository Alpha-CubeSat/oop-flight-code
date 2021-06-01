#ifndef NORMAL_REPORT_MONITOR_HPP_
#define NORMAL_REPORT_MONITOR_HPP_

#include "sfr.hpp"

class NormalReportMonitor : public TimedControlTask<void>{
    public:
        NormalReportMonitor(unsigned int offset);
        void execute();
};

#endif
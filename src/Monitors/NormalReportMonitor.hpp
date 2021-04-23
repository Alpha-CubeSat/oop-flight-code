#ifndef NORMAL_REPORT_MONITOR_HPP_
#define NORMAL_REPORT_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include "Control Tasks/TimedControlTask.hpp"

class NormalReportMonitor : public TimedControlTask<void>{
    public:
        NormalReportMonitor(unsigned int offset);
        void execute();
};

#endif
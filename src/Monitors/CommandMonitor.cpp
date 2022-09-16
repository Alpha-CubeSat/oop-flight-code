#include "CommandMonitor.hpp"
#include "NormalReportMonitor.hpp"

CommandMonitor::CommandMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void CommandMonitor::execute()
{
    // TODO implement- implemented as part of FS-136
}

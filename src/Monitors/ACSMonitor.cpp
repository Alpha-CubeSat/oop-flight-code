#include "ACSMonitor.hpp"

ACSMonitor::ACSMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void ACSMonitor::execute()
{
}

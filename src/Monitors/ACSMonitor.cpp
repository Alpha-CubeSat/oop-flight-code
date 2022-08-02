#include "ACSMonitor.hpp"
#include "IMUMonitor.hpp"

ACSMonitor::ACSMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void ACSMonitor::execute()
{
}

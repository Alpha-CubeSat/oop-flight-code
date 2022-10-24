#include "CommandMonitor.hpp"
#include "NormalReportMonitor.hpp"

CommandMonitor::CommandMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void CommandMonitor::execute()
{
    if (sfr::rockblock::waiting_command) {
        while (!sfr::rockblock::processed_commands.empty()) { // (Only Valid Commands - cmd.isValid())
            RockblockCommand* command = sfr::rockblock::processed_commands.front();
            command->execute(); // Polymorphic Command Execution
            sfr::rockblock::processed_commands.pop_front();
        }
        sfr::rockblock::waiting_command = false;
    }
}

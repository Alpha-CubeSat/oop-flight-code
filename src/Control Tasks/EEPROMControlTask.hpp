#ifndef EEPROM_CONTROL_TASK_HPP_
#define EEPROM_CONTROL_TASK_HPP_

#include "sfr.hpp"
#include <EEPROM.h>

class EEPROMControlTask : public TimedControlTask<void>
{
public:
    EEPROMControlTask(unsigned int offset);
    void execute();
};

#endif
#ifndef EEPROM_CONTROL_TASK_HPP_
#define EEPROM_CONTROL_TASK_HPP_
#include "WString.h"
#include <EEPROM.h>
#include "sfr.hpp"
#include "Control Tasks/TimedControlTask.hpp"

class EEPROMControlTask : public TimedControlTask<void>
{
public:
    EEPROMControlTask(unsigned int offset);
    void execute();
};

#endif
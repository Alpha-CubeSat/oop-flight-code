#ifndef EEPROM_CONTROL_TASK_HPP_
#define EEPROM_CONTROL_TASK_HPP_
#include "Control Tasks/TimedControlTask.hpp"
#include "SFRField.hpp"
#include "WString.h"
#include "sfr.hpp"
#include <EEPROM.h>

class EEPROMControlTask : public TimedControlTask<void>
{
public:
    EEPROMControlTask(unsigned int offset);
    void execute();

private:
    void save_wait_time();
    void save_sfr_data();
};

#endif
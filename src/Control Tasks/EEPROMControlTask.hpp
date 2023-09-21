#ifndef EEPROM_CONTROL_TASK_HPP_
#define EEPROM_CONTROL_TASK_HPP_
#include "Control Tasks/TimedControlTask.hpp"
#include "WString.h"
#include "sfr.hpp"
#include <EEPROM.h>

class EEPROMControlTask : public TimedControlTask<void>
{
public:
    EEPROMControlTask(unsigned int offset);
    void execute();

private:
    void save_boot_time();
    void save_dynamic_data();
    void save_sfr_data();

    uint32_t last_boot_counter_save_time;
    uint32_t last_dynamic_save_time;
    uint32_t last_sfr_save_time;
};

#endif
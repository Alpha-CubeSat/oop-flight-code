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

    bool last_light_switch = false;
    uint8_t fast_cycle_counter; // Counts to constants::eeprom::fast_write_interval before going to 0 and repeating
    uint8_t slow_cycle_counter; // Counts to constants::eeprom::slow_write_interval before going to 0 and repeating
    uint32_t prev_time;
};

#endif
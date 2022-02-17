#include "EEPROMControlTask.hpp"

void EEPROMControlTask::execute()
{
    sfr::EEPROM::curr_time = millis();
    EEPROM.write(0, sfr::EEPROM::curr_time - sfr::EEPROM::start_time + EEPROM.read(0));
}
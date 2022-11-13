#ifndef EEPROM_RESTORE_HPP_
#define EEPROM_RESTORE_HPP_

#include "sfr.hpp"
#include <EEPROM.h>

class EEPROMRestore
{
public:
    static void execute();

private:
    void set_address();
};

#endif
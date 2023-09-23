#ifndef EEPROM_RESTORE_HPP_
#define EEPROM_RESTORE_HPP_
#include "sfr.hpp"
#include <EEPROM.h>

class EEPROMRestore
{
public:
    static void execute();

private:
    static void restore_and_check_boot_vals();
    static void try_restore_and_check_blue_moon_data();
    static void try_restore_dynamic_and_sfr_data();
};

#endif
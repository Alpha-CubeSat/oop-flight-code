#ifndef EEPROM_RESTORE_HPP_
#define EEPROM_RESTORE_HPP_
#include "Control Tasks/EEPROMControlTask.hpp"
#include "sfr.hpp"
#include <EEPROM.h>

class EEPROMRestore
{
public:
    static void execute();

private:
    static void check_boot_vals();
    static void restore_blue_moon_data();
    static void restore_dynamic_data();
    static void restore_sfr_data();
};

#endif
#include "EEPROMRestore.hpp"

void EEPROMRestore::execute()
{
    int addr
}

void EEPROMRestore::set_address()
{
    int addr;
    EEPROM.get(4, addr);
    sfr::EEPROM::read_address = addr;
}

// new plan:

// sfr iterable, dynamically sized data structure of pointers to sfr objects
// using pointer, see object memory location in EEPROM
// update object fields from EEPROM
// write all, but control which ones to restore using presence in data structure
// move entire section when life cycle limit reached
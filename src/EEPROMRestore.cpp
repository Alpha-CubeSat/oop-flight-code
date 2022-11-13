#include "EEPROMRestore.hpp"

void EEPROMRestore::execute()
{
    uint16_t read_addr;
    EEPROM.get(4, read_addr);
    sfr::eeprom::sfr_address = read_addr;

    for (SFRInterface *s : sfr::sfr_fields_vector) {
        read_addr = s->getAddressOffset();
        bool restore;
        EEPROM.get(read_addr, restore);
        if (restore) {
            uint32_t value;
            EEPROM.get(read_addr + 2, value);
            s->setValue(value);
        }
    }
}

// new plan:

// sfr iterable, dynamically sized data structure of pointers to sfr objects
// using pointer, see object memory location in EEPROM
// update object fields from EEPROM
// write all, but control which ones to restore using presence in data structure
// move entire section when life cycle limit reached
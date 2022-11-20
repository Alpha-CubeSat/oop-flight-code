#include "EEPROMRestore.hpp"

void EEPROMRestore::execute()
{
    uint16_t sfr_address;
    EEPROM.get(4, sfr_address);
    sfr::eeprom::sfr_address = sfr_address;

    for (SFRInterface *s : SFRInterface::sfr_fields_vector) {
        int read_address = sfr_address + s->getAddressOffset();
        bool restore;
        EEPROM.get(read_address, restore);
        if (restore) {
            uint32_t value;
            EEPROM.get(read_address + 2, value);
            s->setValue(value); // Virtual funciton, will set the SFRField value with the correct type using child class implementation
        }
    }
}
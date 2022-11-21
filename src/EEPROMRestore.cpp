#include "EEPROMRestore.hpp"

void EEPROMRestore::execute()
{
    uint16_t sfr_address;
    EEPROM.get(5, sfr_address);
    sfr::eeprom::sfr_address = sfr_address;

    uint8_t boot_counter;
    EEPROM.get(4, boot_counter);

    if (boot_counter != 0) {
        // On the very first boot up, do not restore anything from EEPROM and use the default values in the SFR
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

    boot_counter++;
    EEPROM.put(4, boot_counter);
    sfr::eeprom::boot_counter = boot_counter;
}
#include "EEPROMRestore.hpp"

void EEPROMRestore::execute()
{
    // The entire EEPROM memory will be zeroed before launch
    uint8_t boot_counter;
    EEPROM.get(4, boot_counter);

    // On the very first boot up, place the sfr_address value in the appropriate byte. Do not restore anything from EEPROM and use the default values in the SFR
    // On every boot up after, restore SFR values from EEPROM memory
    if (boot_counter == 0) {
        uint16_t sfr_address = sfr::eeprom::sfr_address; // By default, the sfr_address field in the SFR points to the beginning of the first byt section.
        EEPROM.put(5, sfr_address);
    }
    if (boot_counter != 0) {
        uint16_t sfr_address;
        EEPROM.get(5, sfr_address);
        sfr::eeprom::sfr_address = sfr_address;

        for (SFRInterface *s : SFRInterface::sfr_fields_vector) {
            int read_address = sfr_address + s->getAddressOffset();
            bool restore;
            EEPROM.get(read_address, restore);
            s->setRestore(restore);
            if (restore) {
                int data_type = s->getDataType();
                if (data_type == 4) {
                    uint32_t value;
                    EEPROM.get(read_address + 1, value);
                    s->setValue(value);
                }
                else if (data_type == 3) {
                    uint16_t value;
                    EEPROM.get(read_address + 1, value);
                    s->setValue(value);
                }
                else if (data_type == 2) {
                    uint8_t value;
                    EEPROM.get(read_address + 1, value);
                    s->setValue(value);
                }
                else if (data_type == 1) {
                    bool value;
                    EEPROM.get(read_address + 1, value);
                    s->setValue(value);
                }
            }
        }
    }
    boot_counter++;
    EEPROM.put(4, boot_counter);
    sfr::eeprom::boot_counter = boot_counter;
}
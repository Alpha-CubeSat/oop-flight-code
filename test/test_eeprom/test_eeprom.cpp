#include <Control Tasks/EEPROMControlTask.hpp>
#include <EEPROMRestore.hpp>
#include <unity.h>

void resetSFR()
{
    for (auto const &kv : SFRInterface::opcode_lookup) {
        int default_value = SFRInterface::opcode_lookup[kv.first]->getDefaultValue();
        SFRInterface::opcode_lookup[kv.first]->setValue(default_value);
        SFRInterface::opcode_lookup[kv.first]->setRestore(true);
    }
}

void clear_eeprom()
{
    int eeprom_length = EEPROM.length();
    for (int i = 0; i < eeprom_length; i++) {
        EEPROM.write(i, 0);
    }
}

void test_restore_first_boot()
{
    EEPROMControlTask eeprom_control_task(0);

    // Zero out EEPROM bytes
    clear_eeprom();

    // Check boot counter byte in EEPROM is 0 before restore execution
    uint8_t boot_counter;
    EEPROM.get(4, boot_counter);
    TEST_ASSERT_EQUAL(0, boot_counter);

    // Execute EEPROM restore
    EEPROMRestore::execute();

    // Check all SFR fields' values and their restore booleans
    for (auto const &kv : SFRInterface::opcode_lookup) {
        SFRInterface *s = SFRInterface::opcode_lookup[kv.first];
        if (kv.first == 0x2800) {
            // The boot counter in the SFR would have increased from 0 to 1, and its restore boolean is the default true
            TEST_ASSERT_EQUAL(1, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        } else {
            // All other SFR fields would have their default values and their restore booleans are true
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        }
    }

    // Check boot counter byte in EEPROM and SFR is 1 after restore execution
    EEPROM.get(4, boot_counter);
    TEST_ASSERT_EQUAL(1, boot_counter);
    TEST_ASSERT_EQUAL(1, sfr::eeprom::boot_counter);

    // Check that the SFR address was correctly set in EEPROM
    uint16_t sfr_address;
    EEPROM.get(5, sfr_address);
    TEST_ASSERT_EQUAL(sfr::eeprom::sfr_address, sfr_address);
}

void test_restore_general_reboot()
{
    // Setup
    clear_eeprom();
    EEPROMControlTask eeprom_control_task(0);
    EEPROM.put(5, sfr::eeprom::sfr_address); // Change the 5-6 bytes to point to the SFR section in EEPROM
    EEPROM.write(4, 1); // Change the 4th byte (the boot counter) to non-zero, all other EEPROM bytes are unused this test or will be written over
    resetSFR();

    // Set specific SFR fields to be not restored and change their values from default
    sfr::detumble::num_imu_retries.setRestore(false); // Opcode is 0x1502
    sfr::detumble::num_imu_retries = 3;
    sfr::aliveSignal::downlinked.setRestore(false); // Opcode is 0x1601
    sfr::aliveSignal::downlinked = true;
    sfr::mission::acs_transmit_cycle_time.setRestore(false); // OPcode is 0x1800
    sfr::mission::acs_transmit_cycle_time = 90 * constants::time::one_minute;

    // Set specific SFR fields to non-default values (their restore booleans are still true)
    sfr::burnwire::attempts_limit = 15; // Opcode is 0x1905
    sfr::photoresistor::covered = false; // Opcode is 0x1700
    sfr::rockblock::last_downlink = 824; // Opcode is 0x2101

    // Delay for over the write time step
    sfr::eeprom::sfr_last_write_time = 0;
    delay(2000);

    // Check that the conditions for a write are true, execute EEPROM Control Task and check that write endurance and last write time updates
    int initial_sfr_last_write_time = sfr::eeprom::sfr_last_write_time;
    int initial_sfr_address_age = sfr::eeprom::sfr_address_age;
    eeprom_control_task.execute();
    TEST_ASSERT_TRUE(sfr::eeprom::sfr_last_write_time - initial_sfr_last_write_time > sfr::eeprom::sfr_write_step_time);

    // Save the boot_counter before a reboot
    uint8_t boot_counter_before;
    EEPROM.get(4, boot_counter_before);

    // Set all SFR fields to default values and restore booleans to true (as would happen on a reboot)
    resetSFR();

    // Restore SFR values from EEPROM and check that they match from before the delay
    EEPROMRestore::execute();

    for (auto const &kv : SFRInterface::opcode_lookup) {
        SFRInterface *s = SFRInterface::opcode_lookup[kv.first];
        if (kv.first == 0x1502 || kv.first == 0x1601 || kv.first == 0x1800) {
            // Fields should not be restored, so they should hold the default value and their restore boolean should be false
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
            TEST_ASSERT_EQUAL(false, s->getRestore());
        }
        // Fields should be restored, so they should hold the written value and their restore boolean should be true
        else if (kv.first == 0x1905) {
            TEST_ASSERT_EQUAL(15, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        } else if (kv.first == 0x1700) {
            TEST_ASSERT_EQUAL(false, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        } else if (kv.first == 0x2101) {
            TEST_ASSERT_EQUAL(824, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        }
        // The boot counter inthe  SFR would have increased, and its restore boolean is the default true
        else if (kv.first == 0x2800) {
            TEST_ASSERT_EQUAL(boot_counter_before + 1, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        }
        // The boot counter in the SFR would have increased, and its restore boolean is the default true
        else if (kv.first == 0x2805) {
            TEST_ASSERT_EQUAL(initial_sfr_address_age + 1, s-> getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        }
        // Fields should be restored but their values were unchannged, so they should hold the default value and their restore boolean should be true
        else {
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
        }
    }

    // Check boot counter byte in EEPROM has increased after restore execution
    uint8_t boot_counter_after;
    EEPROM.get(4, boot_counter_after);
    TEST_ASSERT_EQUAL(boot_counter_before + 1, boot_counter_after);
}

void test_restore_multiple_writes()
{
    // Setup
    clear_eeprom();
    EEPROMControlTask eeprom_control_task(0);
    EEPROM.put(5, sfr::eeprom::sfr_address); // Change the 5-6 bytes to point to the SFR section in EEPROM
    EEPROM.write(4, 2); // Change the 4th byte (the boot counter) to non-zero, all other EEPROM bytes are unused this test or will be written over
    resetSFR();

    // Set specific SFR fields to be not restored and change their values from default
    sfr::detumble::num_imu_retries.setRestore(false); // Opcode is 0x1502
    sfr::detumble::num_imu_retries = 3;
    sfr::aliveSignal::downlinked.setRestore(false); // Opcode is 0x1601
    sfr::aliveSignal::downlinked = true;
    sfr::mission::acs_transmit_cycle_time.setRestore(false); // OPcode is 0x1800
    sfr::mission::acs_transmit_cycle_time = 90 * constants::time::one_minute;

    // Set specific SFR fields to non-default values (their restore booleans are still true)
    sfr::burnwire::attempts_limit = 15; // Opcode is 0x1905
    sfr::photoresistor::covered = false; // Opcode is 0x1700
    sfr::rockblock::last_downlink = 824; // Opcode is 0x2101

    // Delay for over the write time step
    sfr::eeprom::sfr_last_write_time = 0;
    delay(2000);

    // Check that the conditions for a write are true, execute EEPROM Control Task and check that write endurance and last write time updates
    int initial_sfr_last_write_time = sfr::eeprom::sfr_last_write_time;
    int initial_sfr_address_age = sfr::eeprom::sfr_address_age;
    eeprom_control_task.execute();
    TEST_ASSERT_TRUE(sfr::eeprom::sfr_last_write_time - initial_sfr_last_write_time > sfr::eeprom::sfr_write_step_time);
    TEST_ASSERT_TRUE(sfr::eeprom::sfr_address_age == initial_sfr_address_age + 1);

    // Change the SFR fields, delay, and write again a few times (simulates the CubeSat going through different states)
    sfr::detumble::num_imu_retries = 4;
    sfr::aliveSignal::downlinked = true;
    sfr::mission::acs_transmit_cycle_time = 99 * constants::time::one_minute;
    sfr::burnwire::attempts_limit = 13;
    sfr::photoresistor::covered = true;
    sfr::rockblock::last_downlink = 1012;
    delay(2000);
    initial_sfr_last_write_time = sfr::eeprom::sfr_last_write_time;
    initial_sfr_address_age = sfr::eeprom::sfr_address_age;
    eeprom_control_task.execute();
    TEST_ASSERT_TRUE(sfr::eeprom::sfr_last_write_time - initial_sfr_last_write_time > sfr::eeprom::sfr_write_step_time);
    TEST_ASSERT_TRUE(sfr::eeprom::sfr_address_age == initial_sfr_address_age + 1);

    sfr::detumble::num_imu_retries = 5;
    sfr::aliveSignal::downlinked = true;
    sfr::mission::acs_transmit_cycle_time = 124 * constants::time::one_minute;
    sfr::burnwire::attempts_limit = 20;
    sfr::photoresistor::covered = false;
    sfr::rockblock::last_downlink = 1234;
    delay(2000);
    initial_sfr_last_write_time = sfr::eeprom::sfr_last_write_time;
    initial_sfr_address_age = sfr::eeprom::sfr_address_age;
    eeprom_control_task.execute();
    TEST_ASSERT_TRUE(sfr::eeprom::sfr_last_write_time - initial_sfr_last_write_time > sfr::eeprom::sfr_write_step_time);

    // Save the boot_counter before a reboot
    uint8_t boot_counter_before;
    EEPROM.get(4, boot_counter_before);

    // Set all SFR fields to default values and restore booleans to true (as would happen on a reboot)
    resetSFR();

    // Restore SFR values from EEPROM and check that they match the final write
    EEPROMRestore::execute();

    for (auto const &kv : SFRInterface::opcode_lookup) {
        SFRInterface *s = SFRInterface::opcode_lookup[kv.first];
        if (kv.first == 0x1502 || kv.first == 0x1601 || kv.first == 0x1800) {
            // Fields should not be restored, so they should hold the default value and their restore boolean should be false
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
            TEST_ASSERT_EQUAL(false, s->getRestore());
        }
        // Fields should be restored, so they should hold the written value and their restore boolean should be true
        else if (kv.first == 0x1905) {
            TEST_ASSERT_EQUAL(20, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        } else if (kv.first == 0x1700) {
            TEST_ASSERT_EQUAL(false, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        } else if (kv.first == 0x2101) {
            TEST_ASSERT_EQUAL(1234, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        }
        // The boot counter in the SFR would have increased, and its restore boolean is the default true
        else if (kv.first == 0x2800) {
            TEST_ASSERT_EQUAL(boot_counter_before + 1, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        }
        // The boot counter in the SFR would have increased, and its restore boolean is the default true
        else if (kv.first == 0x2805) {
            TEST_ASSERT_EQUAL(initial_sfr_address_age + 1, s-> getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        }
        // Fields should be restored but their values were unchannged, so they should hold the default value and their restore boolean should be true
        else {
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
        }
    }

    // Check boot counter byte in EEPROM has increased after restore execution
    uint8_t boot_counter_after;
    EEPROM.get(4, boot_counter_after);
    TEST_ASSERT_EQUAL(boot_counter_before + 1, boot_counter_after);
}

void test_restore_write_limit_reboot()
{
    // Setup
    EEPROMControlTask eeprom_control_task(0);

    // Set specific sfr fields to be not restored and change their values from default
    sfr::detumble::num_imu_retries.setRestore(false);
    sfr::detumble::num_imu_retries = 3;
    sfr::aliveSignal::downlinked.setRestore(false);
    sfr::aliveSignal::downlinked = true;
    sfr::mission::acs_transmit_cycle_time.setRestore(false);
    sfr::mission::acs_transmit_cycle_time = 90 * constants::time::one_minute;

    // Set specific sfr fields to non-default values (their restore booleans are still true)
    sfr::burnwire::attempts_limit = 15;
    sfr::photoresistor::covered = false;
    sfr::rockblock::last_downlink = 824;

    // Delay for over the write time step
    delay(2000);

    // Execute EEPROM Control Task at write limit
    sfr::eeprom::sfr_address_age = 99000;
    eeprom_control_task.execute();

    // Check that sfr address age resets to 0
    TEST_ASSERT_EQUAL(0, sfr::eeprom::sfr_address_age);
    uint16_t sfr_address_before = sfr::eeprom::sfr_address;

    // Check that the EEPROM address for SFR data is updated in the SFR and in EEPROM memory
    TEST_ASSERT_EQUAL(sfr_address_before + constants::eeprom::full_offset, sfr::eeprom::sfr_address);
    uint16_t sfr_address_after;
    EEPROM.get(5, sfr_address_after);
    TEST_ASSERT_EQUAL(sfr_address_before + constants::eeprom::full_offset, sfr_address_after);

    // Set all SFR fields to default values and restore booleans to true (as would happen on a reboot)
    resetSFR();

    // Restore SFR values from EEPROM and check that they match the final write
    EEPROMRestore::execute();

    for (SFRInterface *s : SFRInterface::sfr_fields_vector) {
        if (!s == sfr::detumble::num_imu_retries || !s == sfr::aliveSignal::downlinked || !s == sfr::mission::acs_transmit_cycle_time) {
            // Fields should not be restored, so they should hold the default value and their restore boolean should be false
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
            TEST_ASSERT_EQUAL(false, s->getRestore());
        }
        // Fields should be restored, so they should hold the last written value and their restore boolean should be true
        else if (!s == sfr::burnwire::attempts_limit) {
            TEST_ASSERT_EQUAL(20, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        } else if (!s == sfr::photoresistor::covered) {
            TEST_ASSERT_EQUAL(false, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        } else if (!s == sfr::rockblock::last_downlink) {
            TEST_ASSERT_EQUAL(1234, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        }
        // Fields should be restored but their values were unchannged, so they should hold the default value and their restore boolean should be true
        else {
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
        }
    }
}

void test_time_tracker()
{
    EEPROMControlTask eeprom_control_task(0);

    // Delay for over the write time step
    delay(2000);
    // Execute EEPROM Control Task
    eeprom_control_task.execute();

    // Read time value and check that it has advanced

    // Repeat
    // Set wait time to time limit
    // Execute EEPROM Control Task
    // Check that time tracking stops (SFR value)
    // Check that future executes doesn't change the value
}

int test_eeprom()
{
    UNITY_BEGIN();
    // The boot counter and time tracker values in EEPROM memory carry over between test cases.
    RUN_TEST(test_restore_first_boot);
    RUN_TEST(test_restore_general_reboot);
    RUN_TEST(test_restore_multiple_writes);
    // RUN_TEST(test_restore_write_limit_reboot);
    // RUN_TEST(test_time_tracker);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_eeprom();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    Serial.println("EEPROM Test Started");
    test_eeprom();
}

void loop() {}
#endif
#include <Control Tasks/EEPROMControlTask.hpp>
#include <EEPROMRestore.hpp>
#include <Monitors/CameraReportMonitor.hpp>
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

    // Check all SFR fields are default values and their restore boolean is true
    for (SFRInterface *s : SFRInterface::sfr_fields_vector) {
        TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
        TEST_ASSERT_EQUAL(true, s->getRestore());
    }

    // Check boot counter byte in EEPROM is 1 after restore execution
    EEPROM.get(4, boot_counter);
    TEST_ASSERT_EQUAL(1, boot_counter);
}

void test_restore_general_reboot()
{
    EEPROMControlTask eeprom_control_task(0);

    // Set specific sfr fields to be not restored and change their values from default
    sfr::detumble::num_imu_retries.setRestore(false);
    sfr::detumble::num_imu_retries = 3;
    sfr::aliveSignal::downlinked.setRestore(false);
    sfr::aliveSignal::downlinked = true;
    sfr::mission::acs_transmit_cycle_time.setRestore(false);
    sfr::mission::acs_transmit_cycle_time = 90 * constants::time::one_minute;

    // Set specific sfr fields to non-default values
    sfr::burnwire::attempts_limit = 15;
    sfr::photoresistor::covered = false;
    sfr::rockblock::last_downlink = 824;

    // Delay for over the write time step
    delay(2000);

    // Execute EEPROM Control Task
    int initial_sfr_address_age = sfr::eeprom::sfr_address_age.get();
    eeprom_control_task.execute();

    // Check that write endurance increase
    TEST_ASSERT_TRUE(sfr::eeprom::sfr_address_age > initial_sfr_address_age);

    // Set all SFR fields to default values and restore booleans to true (as would happen on a reboot)
    resetSFR();

    // Restore SFR values from EEPROM and check that they match from before the delay
    EEPROMRestore::execute();

    for (SFRInterface *s : SFRInterface::sfr_fields_vector) {
        if (!s == sfr::detumble::num_imu_retries || !s == sfr::aliveSignal::downlinked || !s == sfr::mission::acs_transmit_cycle_time) {
            // Fields should not be restored, so they should hold the default value and their restore boolean should be false
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
            TEST_ASSERT_EQUAL(false, s->getRestore());
        }
        // Fields should be restored, so they should hold the written value and their restore boolean should be true
        else if (!s == sfr::burnwire::attempts_limit) {
            TEST_ASSERT_EQUAL(15, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        } else if (!s == sfr::photoresistor::covered) {
            TEST_ASSERT_EQUAL(false, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        } else if (!s == sfr::rockblock::last_downlink) {
            TEST_ASSERT_EQUAL(824, s->getFieldValue());
            TEST_ASSERT_EQUAL(true, s->getRestore());
        }
        // Fields should be restored but their values were unchannged, so they should hold the default value and their restore boolean should be true
        else {
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
            TEST_ASSERT_EQUAL(s->getDefaultValue(), s->getFieldValue());
        }

        // Check boot counter byte in EEPROM is 1 after restore execution

        EEPROM.get(4, boot_counter);
        TEST_ASSERT_EQUAL(1, boot_counter);
    }

    void test_restore_multiple_writes()
    {
        // Set specific sfr fields to be not restored
        sfr::camera::powered.setRestore(false);
        // Set specific sfr fields to non-default values
        sfr::imu::max_fragments = 128;
        // Delay
        // Execute EEPROM Control Task
        // Repeat above steps x amount of times
        // Check that write endurance increase each write
        // Set all SFR fields to default values and restore booleans to true
        // Restore SFR values from EEPROM and check that they match the final write
    }

    void test_restore_write_limit_reboot()
    {
        // Set specific sfr fields to be not restored
        // Set specific sfr fields to non-default values
        // Delay
        // Execute EEPROM Control Task at write limit
        // Set all SFR fields to default values and restore booleans to true
        // Check that the old sfr address is different from the new sfr address
        // Check that the new sfr address is the same EEPROM is using to read
        // Restore SFR values from EEPROM and check that they match from before the final delay
    }

    void test_time_tracker()
    {
        // Delay
        // Execute EEPROM Control Task
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
        RUN_TEST(test_restore_first_boot);
        RUN_TEST(test_restore_general_reboot);
        RUN_TEST(test_restore_multiple_writes);
        RUN_TEST(test_restore_write_limit_reboot);
        RUN_TEST(test_time_tracker);
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
#include <Control Tasks/EEPROMControlTask.hpp>
#include <EEPROMRestore.hpp>
#include <SFRField.hpp>
#include <unity.h>

/* Zeroes out all EEPROM bytes. */
void reset_eeprom()
{
    uint16_t eeprom_length = EEPROM.length();
    for (uint16_t i = 0; i < eeprom_length; i++) {
        EEPROM.write(i, 0);
    }

    EEPROM.put(constants::eeprom::boot_time_loc1, (uint32_t)0);
    EEPROM.put(constants::eeprom::boot_time_loc2, (uint32_t)0);
    EEPROM.put(constants::eeprom::boot_counter_loc1, (uint8_t)0);
    EEPROM.put(constants::eeprom::boot_counter_loc2, (uint8_t)0);
    EEPROM.put(constants::eeprom::light_switch_loc1, (bool)false);
    EEPROM.put(constants::eeprom::light_switch_loc2, (bool)false);
    EEPROM.put(constants::eeprom::dynamic_data_addr_loc1, (uint16_t)constants::eeprom::dynamic_data_start);
    EEPROM.put(constants::eeprom::dynamic_data_addr_loc2, (uint16_t)constants::eeprom::dynamic_data_start);
    EEPROM.put(constants::eeprom::sfr_data_addr_loc1, (uint16_t)constants::eeprom::sfr_data_start);
    EEPROM.put(constants::eeprom::sfr_data_addr_loc2, (uint16_t)constants::eeprom::sfr_data_start);
}

bool is_eeprom_opcode(int opcode)
{
    if (opcode == 0x2800 || opcode == 0x2801 || opcode == 0x2802 || opcode == 0x2803 || opcode == 0x2804 || opcode == 0x2805 || opcode == 0x2806 || opcode == 0x2807 || opcode == 0x2808 || opcode == 0x2809 || opcode == 0x2810) {
        return true;
    }

    return false;
}

void test_power_cycle_during_boot()
{
    // Setup for first boot
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task1(0);

    // Check EEPROM Restore execute for first boot
    EEPROMRestore::execute();

    TEST_ASSERT_EQUAL(true, sfr::eeprom::boot_mode.get());
    TEST_ASSERT_EQUAL(0, sfr::eeprom::time_alive.get());

    uint8_t boot_counter1;
    EEPROM.get(constants::eeprom::boot_counter_loc1, boot_counter1);
    uint8_t boot_counter2;
    EEPROM.get(constants::eeprom::boot_counter_loc2, boot_counter2);

    TEST_ASSERT_EQUAL(1, sfr::eeprom::boot_counter.get());
    TEST_ASSERT_EQUAL(1, boot_counter1);
    TEST_ASSERT_EQUAL(1, boot_counter2);

    // Save initial time alive value
    uint32_t time_alive = sfr::eeprom::time_alive;

    // Trigger EEPROM Control Task executes like in MCL, end on MCL cycle with write
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval + 1; i++) {
        delay(100); // Approximate delay from MCL cycles
        eeprom_control_task1.execute();
    }

    // Verify EEPROM Control Task
    uint32_t boot_time1;
    EEPROM.get(constants::eeprom::boot_time_loc1, boot_time1);
    uint32_t boot_time2;
    EEPROM.get(constants::eeprom::boot_time_loc2, boot_time2);

    TEST_ASSERT_TRUE(sfr::eeprom::time_alive.get() > time_alive);
    TEST_ASSERT_EQUAL(boot_time1, sfr::eeprom::time_alive.get());
    TEST_ASSERT_EQUAL(boot_time2, sfr::eeprom::time_alive.get());

    // Save most recent time alive value
    time_alive = sfr::eeprom::time_alive;

    // Simulate power cycle
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task2(0);
    EEPROMRestore::execute();

    // Check second restore execution
    TEST_ASSERT_EQUAL(true, sfr::eeprom::boot_mode.get());
    TEST_ASSERT_EQUAL(time_alive, sfr::eeprom::time_alive.get());

    EEPROM.get(constants::eeprom::boot_counter_loc1, boot_counter1);
    EEPROM.get(constants::eeprom::boot_counter_loc2, boot_counter2);

    TEST_ASSERT_EQUAL(2, sfr::eeprom::boot_counter.get());
    TEST_ASSERT_EQUAL(2, boot_counter1);
    TEST_ASSERT_EQUAL(2, boot_counter2);

    // Trigger EEPROM Control Task executes like in MCL, end on MCL cycle with a write
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval + 1; i++) {
        delay(100); // Approximate delay from MCL cycles
        eeprom_control_task2.execute();
    }

    // Verify EEPROM Control Task
    EEPROM.get(constants::eeprom::boot_time_loc1, boot_time1);
    EEPROM.get(constants::eeprom::boot_time_loc2, boot_time2);

    TEST_ASSERT_TRUE(sfr::eeprom::time_alive.get() > time_alive);
    TEST_ASSERT_EQUAL(boot_time1, sfr::eeprom::time_alive.get());
    TEST_ASSERT_EQUAL(boot_time2, sfr::eeprom::time_alive.get());
}

void test_finish_boot()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task(0);
    EEPROM.put(constants::eeprom::boot_time_loc1, (uint32_t)(2 * constants::time::one_hour - 1));
    EEPROM.put(constants::eeprom::boot_time_loc2, (uint32_t)(2 * constants::time::one_hour - 1));
    EEPROM.put(constants::eeprom::boot_counter_loc1, (uint8_t)1);
    EEPROM.put(constants::eeprom::boot_counter_loc2, (uint8_t)1);

    // Check EEPROM Restore execute
    EEPROMRestore::execute();
    TEST_ASSERT_EQUAL(true, sfr::eeprom::boot_mode.get());
    TEST_ASSERT_EQUAL(2 * constants::time::one_hour - 1, sfr::eeprom::time_alive.get());

    uint8_t boot_counter1;
    EEPROM.get(constants::eeprom::boot_counter_loc1, boot_counter1);
    uint8_t boot_counter2;
    EEPROM.get(constants::eeprom::boot_counter_loc2, boot_counter2);

    TEST_ASSERT_EQUAL(2, sfr::eeprom::boot_counter.get());
    TEST_ASSERT_EQUAL(2, boot_counter1);
    TEST_ASSERT_EQUAL(2, boot_counter2);

    // Trigger EEPROM Control Task executes like in MCL
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval; i++) {
        delay(100); // Approximate delay from MCL cycles
        eeprom_control_task.execute();
    }

    TEST_ASSERT_EQUAL(false, sfr::eeprom::boot_mode.get());

    // Simulate power cycle after initial wait is complete
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    TEST_ASSERT_EQUAL(false, sfr::eeprom::boot_mode.get());

    EEPROM.get(constants::eeprom::boot_counter_loc1, boot_counter1);
    EEPROM.get(constants::eeprom::boot_counter_loc2, boot_counter2);

    TEST_ASSERT_EQUAL(3, sfr::eeprom::boot_counter.get());
    TEST_ASSERT_EQUAL(3, boot_counter1);
    TEST_ASSERT_EQUAL(3, boot_counter2);
}

void test_boot_time_error()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROM.put(constants::eeprom::boot_time_loc1, (uint32_t)1234);
    EEPROM.put(constants::eeprom::boot_time_loc2, (uint32_t)4321);
    EEPROM.put(constants::eeprom::boot_counter_loc1, (uint8_t)1);
    EEPROM.put(constants::eeprom::boot_counter_loc2, (uint8_t)1);

    // Try to restore with errored boot values
    EEPROMRestore::execute();

    TEST_ASSERT_EQUAL(true, sfr::eeprom::error_mode.get());
    TEST_ASSERT_EQUAL(true, sfr::eeprom::boot_mode.get());
    TEST_ASSERT_EQUAL(true, sfr::eeprom::boot_restarted.get());

    uint32_t boot_time1;
    EEPROM.get(constants::eeprom::boot_time_loc1, boot_time1);
    uint32_t boot_time2;
    EEPROM.get(constants::eeprom::boot_time_loc2, boot_time2);

    TEST_ASSERT_EQUAL(0, sfr::eeprom::time_alive.get());
    TEST_ASSERT_EQUAL(0, boot_time1);
    TEST_ASSERT_EQUAL(0, boot_time2);

    uint8_t boot_counter1;
    EEPROM.get(constants::eeprom::boot_counter_loc1, boot_counter1);
    uint8_t boot_counter2;
    EEPROM.get(constants::eeprom::boot_counter_loc2, boot_counter2);

    TEST_ASSERT_EQUAL(1, sfr::eeprom::boot_counter.get());
    TEST_ASSERT_EQUAL(1, boot_counter1);
    TEST_ASSERT_EQUAL(1, boot_counter2);
}

void test_blue_moon_data_error()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROM.put(constants::eeprom::boot_time_loc1, (uint32_t)(2 * constants::time::one_hour));
    EEPROM.put(constants::eeprom::boot_time_loc2, (uint32_t)(2 * constants::time::one_hour));
    EEPROM.put(constants::eeprom::boot_counter_loc1, (uint8_t)1);
    EEPROM.put(constants::eeprom::boot_counter_loc2, (uint8_t)1);
    EEPROM.put(constants::eeprom::dynamic_data_addr_loc1, (uint16_t)constants::eeprom::dynamic_data_start);
    EEPROM.put(constants::eeprom::dynamic_data_addr_loc2, (uint16_t)(constants::eeprom::dynamic_data_start + 2));

    // Try to restore with errored blue moon values after boot wait is finished
    EEPROMRestore::execute();

    TEST_ASSERT_EQUAL(true, sfr::eeprom::error_mode.get());

    // Todo: verify that restore does not trigger
    // Plant dynamic data and SFR data in EEPROM, turn light switch on
    // Verify they don't get restored
}

void test_sfr_save_incomplete()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();

    EEPROM.put(constants::eeprom::boot_time_loc1, (uint32_t)(2 * constants::time::one_hour));
    EEPROM.put(constants::eeprom::boot_time_loc2, (uint32_t)(2 * constants::time::one_hour));
    EEPROM.put(constants::eeprom::boot_counter_loc1, (uint8_t)1);
    EEPROM.put(constants::eeprom::boot_counter_loc2, (uint8_t)1);
    EEPROM.put(constants::eeprom::light_switch_loc1, (bool)true);
    EEPROM.put(constants::eeprom::light_switch_loc2, (bool)true);
    EEPROM.put(constants::eeprom::sfr_data_start, (uint32_t)100);
    EEPROM.put(constants::eeprom::sfr_data_start + constants::eeprom::sfr_data_full_offset - 4, (uint32_t)99);

    sfr::stabilization::max_time = 2 * constants::time::one_minute;
    sfr::boot::max_time = 2 * constants::time::one_minute;
    EEPROM.put(constants::eeprom::sfr_data_start + 4, (bool)true);
    EEPROM.put(constants::eeprom::sfr_data_start + 5, sfr::stabilization::max_time.get());
    EEPROM.put(constants::eeprom::sfr_data_start + 9, (bool)true);
    EEPROM.put(constants::eeprom::sfr_data_start + 10, sfr::boot::max_time.get());

    // Simulate power cycle
    // Try to restore after boot wait has fininshed when last SFR write did not complete
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    TEST_ASSERT_EQUAL(false, sfr::eeprom::sfr_save_completed.get());
    TEST_ASSERT_EQUAL(100, sfr::eeprom::sfr_data_age.get());

    // Check that all SFR values are still their defaults
    for (auto const &pair : SFRInterface::opcode_lookup) {
        if (!is_eeprom_opcode(pair.first)) {
            TEST_ASSERT_EQUAL(pair.second->getDefaultValue(), pair.second->getFieldValue());
            TEST_ASSERT_EQUAL(false, pair.second->getRestoreOnBoot());
        }
    }
}

void test_dynamic_data_restore()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task(0);

    // First boot cycle, fastfoward to past boot wait time
    EEPROMRestore::execute();
    sfr::eeprom::time_alive = 2 * constants::time::one_hour;

    // Check that no dynamic data writes have been made
    TEST_ASSERT_EQUAL(0, sfr::eeprom::dynamic_data_age.get());

    // Trigger EEPROM Control Task executes like in MCL for two writes, end on MCL cycle with a write
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval + 1; i++) {
        delay(100); // Approximate delay from MCL cycles
        eeprom_control_task.execute();
    }

    // Save most recent time alive value
    uint32_t time_alive = sfr::eeprom::time_alive;

    // Simulate power cycle
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    // Check second restore execution
    TEST_ASSERT_EQUAL(false, sfr::eeprom::boot_mode.get());
    TEST_ASSERT_EQUAL(false, sfr::eeprom::error_mode.get());
    TEST_ASSERT_EQUAL(time_alive, sfr::eeprom::time_alive.get());
    TEST_ASSERT_EQUAL(2, sfr::eeprom::dynamic_data_age.get());
}

void test_sfr_data_restore()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task(0);

    // First boot cycle, fastfoward to past boot wait time, turn on light switch
    EEPROMRestore::execute();
    sfr::eeprom::time_alive = 2 * constants::time::one_hour;
    sfr::eeprom::light_switch = true;

    // Change some SFR fields
    sfr::burnwire::attempts_limit = 11;
    sfr::burnwire::attempts_limit.setRestoreOnBoot(true);
    sfr::acs::on_time = 0;
    sfr::acs::on_time.setRestoreOnBoot(true);

    // Check that no SFR data writes have been made
    TEST_ASSERT_EQUAL(0, sfr::eeprom::sfr_data_age.get());

    // Trigger EEPROM Control Task executes like in MCL for two writes, end on MCL cycle with a write
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval + 1; i++) {
        delay(100); // Approximate delay from MCL cycles
        eeprom_control_task.execute();
    }

    bool light_switch1;
    EEPROM.get(constants::eeprom::light_switch_loc1, light_switch1);
    bool light_switch2;
    EEPROM.get(constants::eeprom::light_switch_loc2, light_switch2);

    TEST_ASSERT_EQUAL(true, light_switch1);
    TEST_ASSERT_EQUAL(true, light_switch2);

    // Simulate power cycle
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    // Check that all SFR values are still their defaults except for
    for (auto const &pair : SFRInterface::opcode_lookup) {
        if (!is_eeprom_opcode(pair.first) && pair.first != 0x1902 && pair.first != 0x2504) {
            // SFR field is not an EEPROM field or one of changed ones
            TEST_ASSERT_EQUAL(pair.second->getDefaultValue(), pair.second->getFieldValue());
            TEST_ASSERT_EQUAL(false, pair.second->getRestoreOnBoot());
        }
    }

    TEST_ASSERT_EQUAL(11, sfr::burnwire::attempts_limit.get());
    TEST_ASSERT_EQUAL(0, sfr::acs::on_time.get());
    TEST_ASSERT_EQUAL(2, sfr::eeprom::sfr_data_age);
}

void test_light_switch_off()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task(0);

    // First boot cycle, fastfoward to past boot wait time, turn off light switch
    EEPROMRestore::execute();
    sfr::eeprom::time_alive = 2 * constants::time::one_hour;
    sfr::eeprom::light_switch = false;

    // Change some SFR fields
    sfr::burnwire::attempts_limit = 5;
    sfr::burnwire::attempts_limit.setRestoreOnBoot(true);
    sfr::acs::on_time = 20;
    sfr::acs::on_time.setRestoreOnBoot(true);

    // Check that no SFR data writes have been made
    TEST_ASSERT_EQUAL(0, sfr::eeprom::sfr_data_age.get());

    // Trigger EEPROM Control Task executes like in MCL for two writes
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval + 1; i++) {
        delay(100); // Approximate delay from MCL cycles
        eeprom_control_task.execute();
    }

    bool light_switch1;
    EEPROM.get(constants::eeprom::light_switch_loc1, light_switch1);
    bool light_switch2;
    EEPROM.get(constants::eeprom::light_switch_loc2, light_switch2);

    TEST_ASSERT_EQUAL(false, light_switch1);
    TEST_ASSERT_EQUAL(false, light_switch2);

    // Simulate power cycle
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    // Check that all SFR values are still their defaults
    for (auto const &pair : SFRInterface::opcode_lookup) {
        if (!is_eeprom_opcode(pair.first)) {
            TEST_ASSERT_EQUAL(pair.second->getDefaultValue(), pair.second->getFieldValue());
            TEST_ASSERT_EQUAL(false, pair.second->getRestoreOnBoot());
        }
    }
}

int test_eeprom()
{
    UNITY_BEGIN();
    RUN_TEST(test_power_cycle_during_boot);
    RUN_TEST(test_finish_boot);
    RUN_TEST(test_boot_time_error);
    RUN_TEST(test_blue_moon_data_error);
    RUN_TEST(test_sfr_save_incomplete);
    RUN_TEST(test_dynamic_data_restore);
    RUN_TEST(test_sfr_data_restore);
    RUN_TEST(test_light_switch_off);
    // RUN_TEST(test_save_with_full_eeprom);
    // RUN_TEST(test_restore_after_full_eeprom);

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
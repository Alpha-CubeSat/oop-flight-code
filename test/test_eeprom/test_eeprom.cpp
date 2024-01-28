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
    EEPROMControlTask eeprom_control_task1;

    // Check EEPROM Restore execute for first boot
    EEPROMRestore::execute();

    TEST_ASSERT_EQUAL(true, sfr::eeprom::boot_mode.get()); // We are in boot mode
    TEST_ASSERT_EQUAL(0, sfr::eeprom::time_alive.get());   // Time alive has not started counting yet

    uint8_t boot_counter1;
    EEPROM.get(constants::eeprom::boot_counter_loc1, boot_counter1);
    uint8_t boot_counter2;
    EEPROM.get(constants::eeprom::boot_counter_loc2, boot_counter2);

    TEST_ASSERT_EQUAL(1, sfr::eeprom::boot_counter.get()); // Check boot counter increased to 1
    TEST_ASSERT_EQUAL(1, boot_counter1);                   // Check first boot counter in EEPROM updated properly
    TEST_ASSERT_EQUAL(1, boot_counter2);                   // check second boot counter in EEPROM updated properly

    // Save initial time alive value
    uint32_t initial_time_alive = sfr::eeprom::time_alive;

    // Trigger EEPROM Control Task executes like in MCL, end on MCL cycle with write (2 writes total)
    uint32_t time_passed = 0;
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval + 1; i++) {
        eeprom_control_task1.execute();

        // Simulate MCL cycle time to increase time alive counter
        // delay(constants::time::control_cycle_time_ms); // For some reason, PlatformIO doesn't like this line
        time_passed += constants::time::control_cycle_time_ms;
        sfr::eeprom::time_alive = time_passed;
    }

    // Save most recent time alive value written to EEPROM
    uint32_t saved_time_alive = sfr::eeprom::time_alive - constants::time::control_cycle_time_ms;

    // Verify EEPROM Control Task
    uint32_t boot_time1;
    EEPROM.get(constants::eeprom::boot_time_loc1, boot_time1);
    uint32_t boot_time2;
    EEPROM.get(constants::eeprom::boot_time_loc2, boot_time2);

    TEST_ASSERT_TRUE(boot_time1 > initial_time_alive); // Check that boot timer has increased
    TEST_ASSERT_EQUAL(saved_time_alive, boot_time1);   // Check that the first boot time value in EEPROM updated properly
    TEST_ASSERT_EQUAL(saved_time_alive, boot_time2);   // Check that the first boot time value in EEPROM updated properly

    // Simulate power cycle
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task2;
    EEPROMRestore::execute();

    // Check second restore execution
    TEST_ASSERT_EQUAL(true, sfr::eeprom::boot_mode.get());        // Check that we are still in boot mode (time alive should not have exceeded the boot timeout)
    TEST_ASSERT_EQUAL(boot_time1, sfr::eeprom::time_alive.get()); // Check that the time alive value was properly restored into the SFR

    EEPROM.get(constants::eeprom::boot_counter_loc1, boot_counter1);
    EEPROM.get(constants::eeprom::boot_counter_loc2, boot_counter2);

    TEST_ASSERT_EQUAL(2, sfr::eeprom::boot_counter.get()); // Check that the boot counter has increased
    TEST_ASSERT_EQUAL(2, boot_counter1);                   // Check that the first boot counter in EEPROM updated properly
    TEST_ASSERT_EQUAL(2, boot_counter2);                   // Check that the second boot counter in EEPROM updated properly

    // Save initial time alive value a
    initial_time_alive = sfr::eeprom::time_alive;

    // Trigger EEPROM Control Task executes like in MCL, end on MCL cycle with write (2 writes total)
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval + 1; i++) {
        eeprom_control_task2.execute();

        // Simulate MCL cycle time to increase time alive counter
        // delay(constants::time::control_cycle_time_ms); // For some reason, PlatformIO doesn't like this line
        time_passed += constants::time::control_cycle_time_ms;
        sfr::eeprom::time_alive = time_passed;
    }

    // Save most recent time alive value written to EEPROM
    saved_time_alive = sfr::eeprom::time_alive - constants::time::control_cycle_time_ms;

    // Verify EEPROM Control Task continues increasing boot time after power cycle
    EEPROM.get(constants::eeprom::boot_time_loc1, boot_time1);
    EEPROM.get(constants::eeprom::boot_time_loc2, boot_time2);

    TEST_ASSERT_TRUE(boot_time1 > initial_time_alive); // Check that boot timer has increased
    TEST_ASSERT_EQUAL(saved_time_alive, boot_time1);   // Check that the first boot time value in EEPROM updated properly
    TEST_ASSERT_EQUAL(saved_time_alive, boot_time2);   // Check that the first boot time value in EEPROM updated properly
}

void test_finish_boot()
{
    // Setup to right before boot phase will end (first boot has already occured)
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task;
    EEPROM.put(constants::eeprom::boot_time_loc1, (uint32_t)(sfr::boot::max_time.get() - 1));
    EEPROM.put(constants::eeprom::boot_time_loc2, (uint32_t)(sfr::boot::max_time.get() - 1));
    EEPROM.put(constants::eeprom::boot_counter_loc1, (uint8_t)1);
    EEPROM.put(constants::eeprom::boot_counter_loc2, (uint8_t)1);

    // Simulate power cycle and check EEPROM Restore execute
    EEPROMRestore::execute();
    TEST_ASSERT_EQUAL(true, sfr::eeprom::boot_mode.get());
    TEST_ASSERT_EQUAL(sfr::boot::max_time.get() - 1, sfr::eeprom::time_alive.get());

    uint8_t boot_counter1;
    EEPROM.get(constants::eeprom::boot_counter_loc1, boot_counter1);
    uint8_t boot_counter2;
    EEPROM.get(constants::eeprom::boot_counter_loc2, boot_counter2);

    TEST_ASSERT_EQUAL(2, sfr::eeprom::boot_counter.get());
    TEST_ASSERT_EQUAL(2, boot_counter1);
    TEST_ASSERT_EQUAL(2, boot_counter2);

    // Trigger EEPROM Control Task executes like in MCL, end on MCL cycle with write (2 writes total)
    uint32_t time_passed = 0;
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval + 1; i++) {
        eeprom_control_task.execute();

        // Simulate MCL cycle time to increase time alive counter
        // delay(constants::time::control_cycle_time_ms); // For some reason, PlatformIO doesn't like this line
        time_passed += constants::time::control_cycle_time_ms;
        sfr::eeprom::time_alive = time_passed;
    }

    TEST_ASSERT_EQUAL(false, sfr::eeprom::boot_mode.get()); // Check that the boot phase has finished

    // Simulate power cycle after initial wait is complete
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    // Check that the boot phase has finished after power cycle
    TEST_ASSERT_EQUAL(false, sfr::eeprom::boot_mode.get());

    // Check that the boot counters have updated properly
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
    EEPROM.put(constants::eeprom::boot_time_loc2, (uint32_t)4321); // Purposely does not equal first boot time value to simulate error
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

    TEST_ASSERT_EQUAL(1, sfr::eeprom::boot_counter.get()); // Check that the boot counter gets reset back to 1 (previous counter is no longer trusted)
    TEST_ASSERT_EQUAL(1, boot_counter1);
    TEST_ASSERT_EQUAL(1, boot_counter2);
}

void test_blue_moon_data_error()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROM.put(constants::eeprom::boot_time_loc1, (uint32_t)sfr::boot::max_time.get());
    EEPROM.put(constants::eeprom::boot_time_loc2, (uint32_t)sfr::boot::max_time.get());
    EEPROM.put(constants::eeprom::boot_counter_loc1, (uint8_t)1);
    EEPROM.put(constants::eeprom::boot_counter_loc2, (uint8_t)1);
    EEPROM.put(constants::eeprom::dynamic_data_addr_loc1, (uint16_t)constants::eeprom::dynamic_data_start);
    EEPROM.put(constants::eeprom::dynamic_data_addr_loc2, (uint16_t)(constants::eeprom::dynamic_data_start + 1)); // Purposely does not equal first dynamc data start to simulate error

    sfr::eeprom::time_alive = sfr::boot::max_time.get() + 1; // Change stored dynamic data
    sfr::eeprom::light_switch = true;                            // Turn light switch on

    // Change some SFR fields
    sfr::camera::failed_limit = 10;
    sfr::camera::failed_limit.setRestoreOnBoot(true);
    sfr::rockblock::downlink_period = 15 * constants::time::one_minute;
    sfr::rockblock::downlink_period.setRestoreOnBoot(true);

    // Write dynamic data and SFR data
    EEPROMControlTask eeprom_control_task;
    eeprom_control_task.execute();

    // Simulate power cycle
    // Try to restore with errored blue moon values after boot wait is finished
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    TEST_ASSERT_EQUAL(true, sfr::eeprom::error_mode.get()); // An error has been detected

    // Verify SFR data and dynamic data do not get restored
    TEST_ASSERT_EQUAL(sfr::eeprom::time_alive.getDefaultValue(), sfr::eeprom::time_alive.getFieldValue());
    TEST_ASSERT_EQUAL(sfr::eeprom::dynamic_data_age.getDefaultValue(), sfr::eeprom::dynamic_data_age.getFieldValue());

    for (auto const &pair : SFRInterface::opcode_lookup) {
        if (!is_eeprom_opcode(pair.first)) {
            TEST_ASSERT_EQUAL(pair.second->getDefaultValue(), pair.second->getFieldValue());
            TEST_ASSERT_EQUAL(false, pair.second->getRestoreOnBoot());
        }
    }
}

void test_sfr_save_error()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task;

    EEPROM.put(constants::eeprom::boot_time_loc1, (uint32_t)sfr::boot::max_time.get());
    EEPROM.put(constants::eeprom::boot_time_loc2, (uint32_t)sfr::boot::max_time.get());
    EEPROM.put(constants::eeprom::boot_counter_loc1, (uint8_t)1);
    EEPROM.put(constants::eeprom::boot_counter_loc2, (uint8_t)1);
    EEPROM.put(constants::eeprom::light_switch_loc1, (bool)true);
    EEPROM.put(constants::eeprom::light_switch_loc2, (bool)true);

    // Perform an SFR save
    eeprom_control_task.execute();

    // Corrupt some SFR values in EEPROM
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
    EEPROMControlTask eeprom_control_task;

    // First boot cycle, fastfoward to past boot wait time
    EEPROMRestore::execute();
    sfr::eeprom::time_alive = sfr::boot::max_time.get();

    // Check that no dynamic data writes have been made
    TEST_ASSERT_EQUAL(0, sfr::eeprom::dynamic_data_age.get());

    // Trigger EEPROM Control Task executes like in MCL, end on MCL cycle with write (2 writes total)
    uint32_t time_passed = 0;
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval + 1; i++) {
        eeprom_control_task.execute();

        // Simulate MCL cycle time to increase time alive counter
        // delay(constants::time::control_cycle_time_ms); // For some reason, PlatformIO doesn't like this line
        time_passed += constants::time::control_cycle_time_ms;
        sfr::eeprom::time_alive = time_passed;
    }

    // Save most recent time alive value written to EEPROM
    uint32_t saved_time_alive = sfr::eeprom::time_alive - constants::time::control_cycle_time_ms;

    // Simulate power cycle
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    // Check second restore execution
    TEST_ASSERT_EQUAL(false, sfr::eeprom::boot_mode.get());
    TEST_ASSERT_EQUAL(false, sfr::eeprom::error_mode.get());
    TEST_ASSERT_EQUAL(saved_time_alive, sfr::eeprom::time_alive.get());
    TEST_ASSERT_EQUAL(2, sfr::eeprom::dynamic_data_age.get());
}

void test_sfr_data_restore()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task;

    // First boot cycle, fastfoward to past boot wait time, turn on light switch
    EEPROMRestore::execute();
    sfr::eeprom::time_alive = sfr::boot::max_time.get();
    sfr::eeprom::light_switch = true;

    // Change some SFR fields
    sfr::burnwire::attempts_limit = 11;
    sfr::burnwire::attempts_limit.setRestoreOnBoot(true);
    sfr::acs::on_time = 0;
    sfr::acs::on_time.setRestoreOnBoot(true);

    // Check that no SFR data writes have been made
    TEST_ASSERT_EQUAL(0, sfr::eeprom::sfr_data_age.get());

    // Trigger EEPROM Control Task executes like in MCL, end on MCL cycle with write (2 writes total)
    uint32_t time_passed = 0;
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval + 1; i++) {
        eeprom_control_task.execute();

        // Simulate MCL cycle time to increase time alive counter
        // delay(constants::time::control_cycle_time_ms); // For some reason, PlatformIO doesn't like this line
        time_passed += constants::time::control_cycle_time_ms;
        sfr::eeprom::time_alive = time_passed;
    }

    // Check that light switches have been set
    bool light_switch1;
    EEPROM.get(constants::eeprom::light_switch_loc1, light_switch1);
    bool light_switch2;
    EEPROM.get(constants::eeprom::light_switch_loc2, light_switch2);

    TEST_ASSERT_EQUAL(true, light_switch1);
    TEST_ASSERT_EQUAL(true, light_switch2);

    // Simulate power cycle
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    // Check that all SFR values are still their defaults except for the changed ones
    for (auto const &pair : SFRInterface::opcode_lookup) {
        if (!is_eeprom_opcode(pair.first) && pair.first != 0x1902 && pair.first != 0x2504) {
            // SFR field is not an EEPROM field or one of changed ones
            TEST_ASSERT_EQUAL(pair.second->getDefaultValue(), pair.second->getFieldValue());
            TEST_ASSERT_EQUAL(false, pair.second->getRestoreOnBoot());
        }
    }

    TEST_ASSERT_EQUAL(true, sfr::eeprom::sfr_save_completed.get());
    TEST_ASSERT_EQUAL(11, sfr::burnwire::attempts_limit.get());
    TEST_ASSERT_EQUAL(0, sfr::acs::on_time.get());
    TEST_ASSERT_EQUAL(2, sfr::eeprom::sfr_data_age); // Write age increased by 2
}

void test_light_switch_off()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task;

    // First boot cycle, fastfoward to past boot wait time, turn off light switch
    EEPROMRestore::execute();
    sfr::eeprom::time_alive = sfr::boot::max_time.get();
    sfr::eeprom::light_switch = false;

    // Change some SFR fields
    sfr::burnwire::attempts_limit = 5;
    sfr::burnwire::attempts_limit.setRestoreOnBoot(true);
    sfr::acs::on_time = 20;
    sfr::acs::on_time.setRestoreOnBoot(true);

    // Check that no SFR data writes have been made
    TEST_ASSERT_EQUAL(0, sfr::eeprom::sfr_data_age.get());

    // Trigger EEPROM Control Task executes like in MCL, end on MCL cycle with write (2 writes total)
    uint32_t time_passed = 0;
    for (unsigned int i = 0; i < constants::eeprom::slow_write_interval + 1; i++) {
        eeprom_control_task.execute();

        // Simulate MCL cycle time to increase time alive counter
        // delay(constants::time::control_cycle_time_ms); // For some reason, PlatformIO doesn't like this line
        time_passed += constants::time::control_cycle_time_ms;
        sfr::eeprom::time_alive = time_passed;
    }

    // Check that light switches have been set
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

void test_dynamic_age_limit()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task;

    // First boot cycle, fastfoward to past boot wait time
    EEPROMRestore::execute();
    sfr::eeprom::time_alive = sfr::boot::max_time.get();

    // Check the current dynamic data address
    TEST_ASSERT_EQUAL(constants::eeprom::dynamic_data_start, sfr::eeprom::dynamic_data_addr.get());

    // Set the dynamic data age to be near the limit
    sfr::eeprom::dynamic_data_age = constants::eeprom::write_age_limit - 1;

    // Trigger EEPROM Control Task executes like in MCL, end on MCL cycle with write (2 writes total)
    uint32_t time_passed = 0;
    for (unsigned int i = 0; i < constants::eeprom::fast_write_interval + 1; i++) {
        eeprom_control_task.execute();

        // Simulate MCL cycle time to increase time alive counter
        // delay(constants::time::control_cycle_time_ms); // For some reason, PlatformIO doesn't like this line
        time_passed += constants::time::control_cycle_time_ms;
        sfr::eeprom::time_alive = time_passed;
    }

    // Save most recent time alive value written to EEPROM
    uint32_t saved_time_alive = sfr::eeprom::time_alive - constants::time::control_cycle_time_ms;

    // Check that dynamic data address and age updated
    TEST_ASSERT_EQUAL(constants::eeprom::dynamic_data_start + constants::eeprom::dynamic_data_full_offset, sfr::eeprom::dynamic_data_addr.get());
    TEST_ASSERT_EQUAL(1, sfr::eeprom::dynamic_data_age.get());

    // Simulate power cycle
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    // Check restore execution for dynamic data
    TEST_ASSERT_EQUAL(saved_time_alive, sfr::eeprom::time_alive.get());
    TEST_ASSERT_EQUAL(constants::eeprom::dynamic_data_start + constants::eeprom::dynamic_data_full_offset, sfr::eeprom::dynamic_data_addr.get());
    TEST_ASSERT_EQUAL(1, sfr::eeprom::dynamic_data_age.get());
}

void test_sfr_age_limit()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task;

    // First boot cycle, fastfoward to past boot wait time
    EEPROMRestore::execute();
    sfr::eeprom::time_alive = sfr::boot::max_time.get();

    // Check the current SFR data address
    TEST_ASSERT_EQUAL(constants::eeprom::sfr_data_start, sfr::eeprom::sfr_data_addr.get());

    // Set the dynamic data age to be near the limit
    sfr::eeprom::sfr_data_age = constants::eeprom::write_age_limit - 1;

    // Trigger EEPROM Control Task executes like in MCL, end on MCL cycle with write (2 writes total)
    uint32_t time_passed = 0;
    for (unsigned int i = 0; i < constants::eeprom::slow_write_interval + 1; i++) {
        eeprom_control_task.execute();

        // Simulate MCL cycle time to increase time alive counter
        // delay(constants::time::control_cycle_time_ms); // For some reason, PlatformIO doesn't like this line
        time_passed += constants::time::control_cycle_time_ms;
        sfr::eeprom::time_alive = time_passed;
    }

    // Check that dynamic data address and age updated
    TEST_ASSERT_EQUAL(constants::eeprom::sfr_data_start + constants::eeprom::sfr_data_full_offset, sfr::eeprom::sfr_data_addr.get());
    TEST_ASSERT_EQUAL(1, sfr::eeprom::sfr_data_age.get());

    // Simulate power cycle
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    // Check restore execution for sfr eeprom metadata
    TEST_ASSERT_EQUAL(constants::eeprom::sfr_data_start + constants::eeprom::sfr_data_full_offset, sfr::eeprom::sfr_data_addr.get());
    TEST_ASSERT_EQUAL(1, sfr::eeprom::sfr_data_age.get());
}

void test_save_restore_with_full_eeprom()
{
    // Setup
    reset_eeprom();
    SFRInterface::resetSFR();
    EEPROMControlTask eeprom_control_task;

    // First boot cycle, fastfoward to past boot wait time
    EEPROMRestore::execute();
    sfr::eeprom::time_alive = sfr::boot::max_time.get();

    // Set the current SFR data address to the last section (7th section, or 6 offsets)
    sfr::eeprom::sfr_data_addr = constants::eeprom::sfr_data_start + 6 * constants::eeprom::sfr_data_full_offset;
    sfr::eeprom::sfr_data_age = constants::eeprom::write_age_limit;

    // Set the current dynamic data address to the last section (56th section, or 55 offsets)
    sfr::eeprom::dynamic_data_addr = constants::eeprom::dynamic_data_start + 55 * constants::eeprom::dynamic_data_full_offset;
    sfr::eeprom::dynamic_data_age = constants::eeprom::write_age_limit;

    // Trigger EEPROM Control Task
    eeprom_control_task.execute();

    // Check that dynamic and sfr section ages do not change
    TEST_ASSERT_EQUAL(constants::eeprom::dynamic_data_start + 56 * constants::eeprom::dynamic_data_full_offset, sfr::eeprom::dynamic_data_addr.get());
    TEST_ASSERT_EQUAL(constants::eeprom::sfr_data_start + 7 * constants::eeprom::sfr_data_full_offset, sfr::eeprom::sfr_data_addr.get());
    TEST_ASSERT_EQUAL(0, sfr::eeprom::dynamic_data_age.get());
    TEST_ASSERT_EQUAL(0, sfr::eeprom::sfr_data_age.get());

    // Simulate power cycle
    SFRInterface::resetSFR();
    EEPROMRestore::execute();

    // Check restore execution
    TEST_ASSERT_EQUAL(0, sfr::eeprom::time_alive.get());
    TEST_ASSERT_EQUAL(constants::eeprom::dynamic_data_start + 56 * constants::eeprom::dynamic_data_full_offset, sfr::eeprom::dynamic_data_addr.get());
    TEST_ASSERT_EQUAL(constants::eeprom::sfr_data_start + 7 * constants::eeprom::sfr_data_full_offset, sfr::eeprom::sfr_data_addr.get());
    TEST_ASSERT_EQUAL(0, sfr::eeprom::dynamic_data_age.get());
    TEST_ASSERT_EQUAL(0, sfr::eeprom::sfr_data_age.get());
}

int test_eeprom()
{
    UNITY_BEGIN();
    RUN_TEST(test_power_cycle_during_boot);
    RUN_TEST(test_finish_boot);
    RUN_TEST(test_boot_time_error);
    RUN_TEST(test_blue_moon_data_error);
    RUN_TEST(test_sfr_save_error);
    RUN_TEST(test_dynamic_data_restore);
    RUN_TEST(test_sfr_data_restore);
    RUN_TEST(test_light_switch_off);
    RUN_TEST(test_dynamic_age_limit);
    RUN_TEST(test_sfr_age_limit);
    RUN_TEST(test_save_restore_with_full_eeprom);

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
    test_eeprom();
}

void loop() {}
#endif
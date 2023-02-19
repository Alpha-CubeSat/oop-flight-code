#include "Monitors/CommandMonitor.hpp"
#include "Monitors/NormalReportMonitor.hpp"
#include "RockblockSimulator.hpp"
#include "sfr.hpp"
#include <unity.h>

void test_normal_report_without_commands()
{
    NormalReportMonitor normal_report_monitor(0);
    normal_report_monitor.execute();
    TEST_ASSERT_EQUAL(31, sfr::rockblock::normal_report.size());
}

void test_normal_report_fault()
{
    NormalReportMonitor normal_report_monitor(0);
    sfr::imu::mag_x_average->set_invalid();
    sfr::temperature::temp_c_average->set_invalid();
    normal_report_monitor.execute();
    TEST_ASSERT_EQUAL(0b10111110, sfr::rockblock::normal_report[27]);
    TEST_ASSERT_EQUAL(31, sfr::rockblock::normal_report.size());
}
/*
void test_normal_report_with_one_command()
{
    NormalReportMonitor normal_report_monitor(0);
    CommandMonitor command_monitor(0);
    //RawRockblockCommand new_raw_command;
    //new_raw_command.opcode[0] = constants::rockblock::opcodes::sfr_field_opcode_arm>>8;
    //new_raw_command.opcode[1] = constants::rockblock::opcodes::sfr_field_opcode_arm<<8;
    uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
    uint32_t f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::deployment);
    RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    normal_report_monitor.execute();
    TEST_ASSERT_EQUAL(33, sfr::rockblock::normal_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[0], sfr::rockblock::normal_report[29]);
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[1], sfr::rockblock::normal_report[30]);
}

void test_normal_report_with_multiple_commands()
{
    NormalReportMonitor normal_report_monitor(0);
    CommandMonitor command_monitor(0);
    uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
    uint32_t f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::deployment);
    RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_arm);
    f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    normal_report_monitor.execute();
    TEST_ASSERT_EQUAL(35, sfr::rockblock::normal_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[0], sfr::rockblock::normal_report[29]);
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[1], sfr::rockblock::normal_report[30]);
    TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[0], sfr::rockblock::normal_report[31]);
    TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[1], sfr::rockblock::normal_report[32]);
    TEST_ASSERT_EQUAL(constants::rockblock::end_of_normal_downlink_flag1, sfr::rockblock::normal_report[33]);
    TEST_ASSERT_EQUAL(constants::rockblock::end_of_normal_downlink_flag2, sfr::rockblock::normal_report[34]);
}

void test_normal_report_with_more_than_15_commands()
{
    NormalReportMonitor normal_report_monitor(0);
    CommandMonitor command_monitor(0);
    for (int i = 0; i < 30; i++) {
        uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_arm);
        uint32_t f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
        RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
        sfr::rockblock::processed_commands.push_back(command);
    }
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    normal_report_monitor.execute();
    TEST_ASSERT_EQUAL(62, sfr::rockblock::normal_report.size());
    for (int i = 0; i < 15; i++) {
        TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[0], sfr::rockblock::normal_report[30 + (i * 2)]);
        TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[1], sfr::rockblock::normal_report[30 + (i * 2) + 1]);
    }
    TEST_ASSERT_EQUAL(constants::rockblock::end_of_normal_downlink_flag1, sfr::rockblock::normal_report[60]);
    TEST_ASSERT_EQUAL(constants::rockblock::end_of_normal_downlink_flag2, sfr::rockblock::normal_report[61]);
}
*/
void test_normal_report_serialize()
{
    NormalReportMonitor normal_report_monitor(0);
    sfr::photoresistor::covered = true;
    sfr::button::pressed = true;
    sfr::mission::current_mode = sfr::mission::bootCamera;
    sfr::burnwire::burn_time = SFRField<uint32_t>(3000, 0x1902, constants::eeprom::burnwire_burn_time_offset, true);
    sfr::burnwire::armed_time = 432000000;
    sfr::burnwire::mode = (uint16_t)burnwire_mode_type::delay;
    sfr::burnwire::attempts = 5;
    sfr::rockblock::downlink_period = 600000;
    sfr::rockblock::waiting_command = false;
    sfr::rockblock::waiting_message = false;

    sfr::imu::mag_z_average->set_valid();
    sfr::imu::mag_x_average->set_valid();
    sfr::imu::mag_y_average->set_valid();
    sfr::imu::gyro_z_average->set_valid();
    sfr::imu::gyro_x_average->set_valid();
    sfr::imu::gyro_y_average->set_valid();
    sfr::current::solar_current_average->set_valid();
    sfr::temperature::temp_c_average->set_valid();
    sfr::photoresistor::light_val_average_standby->set_valid();
    sfr::battery::voltage_average->set_valid();
    sfr::current::in_sun = true;
    faults::fault_1 = 0;
    faults::fault_2 = 0;
    faults::fault_3 = 0;
    sfr::camera::powered = false;
    sfr::eeprom::boot_counter = 7;

    normal_report_monitor.execute();
    for (int i = 0; i < sfr::rockblock::normal_report.size(); i++) {
        Serial.println(sfr::rockblock::normal_report[i]);
    }

    TEST_ASSERT_EQUAL(99, sfr::rockblock::normal_report[0]);
    TEST_ASSERT_EQUAL(1, sfr::rockblock::normal_report[1]);
    TEST_ASSERT_EQUAL(1, sfr::rockblock::normal_report[2]);
    TEST_ASSERT_EQUAL(18, sfr::rockblock::normal_report[3]);
    TEST_ASSERT_EQUAL(153, sfr::rockblock::normal_report[4]);
    TEST_ASSERT_EQUAL(127, sfr::rockblock::normal_report[5]);
    TEST_ASSERT_EQUAL(2, sfr::rockblock::normal_report[6]);
    TEST_ASSERT_EQUAL(5, sfr::rockblock::normal_report[7]);
    TEST_ASSERT_EQUAL(0, sfr::rockblock::normal_report[8]);
    TEST_ASSERT_EQUAL(0, sfr::rockblock::normal_report[9]);
    TEST_ASSERT_EQUAL(0, sfr::rockblock::normal_report[10]);
    TEST_ASSERT_EQUAL(127, sfr::rockblock::normal_report[11]);
    TEST_ASSERT_EQUAL(127, sfr::rockblock::normal_report[12]);
    TEST_ASSERT_EQUAL(127, sfr::rockblock::normal_report[13]);
    TEST_ASSERT_EQUAL(127, sfr::rockblock::normal_report[14]);
    TEST_ASSERT_EQUAL(127, sfr::rockblock::normal_report[15]);
    TEST_ASSERT_EQUAL(127, sfr::rockblock::normal_report[16]);
    TEST_ASSERT_EQUAL(0, sfr::rockblock::normal_report[17]);
    TEST_ASSERT_EQUAL(51, sfr::rockblock::normal_report[18]);
    TEST_ASSERT_EQUAL(0, sfr::rockblock::normal_report[19]);
    TEST_ASSERT_EQUAL(1, sfr::rockblock::normal_report[20]);
    TEST_ASSERT_EQUAL(0, sfr::rockblock::normal_report[21]);
    TEST_ASSERT_EQUAL(0, sfr::rockblock::normal_report[22]);
    TEST_ASSERT_EQUAL(0, sfr::rockblock::normal_report[23]);
    TEST_ASSERT_EQUAL(0, sfr::rockblock::normal_report[24]);
    TEST_ASSERT_EQUAL(0, sfr::rockblock::normal_report[25]);
    TEST_ASSERT_EQUAL(7, sfr::rockblock::normal_report[26]);
    TEST_ASSERT_EQUAL(255, sfr::rockblock::normal_report[27]);
    TEST_ASSERT_EQUAL(3, sfr::rockblock::normal_report[28]);
    TEST_ASSERT_EQUAL(254, sfr::rockblock::normal_report[29]);
    TEST_ASSERT_EQUAL(255, sfr::rockblock::normal_report[30]);
}

int test_normal_report_acknowledge()
{
    UNITY_BEGIN();
    RUN_TEST(test_normal_report_fault);
    RUN_TEST(test_normal_report_without_commands);
    RUN_TEST(test_normal_report_serialize);
    // RUN_TEST(test_normal_report_with_one_command);
    //   RUN_TEST(test_normal_report_with_multiple_commands);
    //   RUN_TEST(test_normal_report_with_more_than_15_commands);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_normal_report_acknowledge();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    test_normal_report_acknowledge();
}

void loop() {}
#endif
#include "Monitors/CommandMonitor.hpp"
#include "Monitors/NormalReportMonitor.hpp"
#include "RockblockSimulator.hpp"
#include "sfr.hpp"
#include <unity.h>

void test_normal_report_without_commands()
{
    NormalReportMonitor normal_report_monitor(0);
    normal_report_monitor.execute();
    TEST_ASSERT_EQUAL(34, sfr::rockblock::normal_report.size());
}

void test_normal_report_with_one_command()
{
    NormalReportMonitor normal_report_monitor(0);
    CommandMonitor command_monitor(0);
    uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
    uint32_t f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::deployment);
    RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    normal_report_monitor.execute();
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[0], sfr::rockblock::normal_report[32]);
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[1], sfr::rockblock::normal_report[33]);
    TEST_ASSERT_EQUAL(36, sfr::rockblock::normal_report.size());
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
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[0], sfr::rockblock::normal_report[32]);
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[1], sfr::rockblock::normal_report[33]);
    TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[0], sfr::rockblock::normal_report[34]);
    TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[1], sfr::rockblock::normal_report[35]);
    TEST_ASSERT_EQUAL(38, sfr::rockblock::normal_report.size());
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
    for (int i = 0; i < 15; i++) {
        TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[0], sfr::rockblock::normal_report[38 + (i * 2)]);
        TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[1], sfr::rockblock::normal_report[38 + (i * 2) + 1]);
    }
    TEST_ASSERT_EQUAL(constants::rockblock::end_of_normal_downlink_flag1, sfr::rockblock::normal_report[62]);
    TEST_ASSERT_EQUAL(constants::rockblock::end_of_normal_downlink_flag2, sfr::rockblock::normal_report[63]);
    TEST_ASSERT_EQUAL(64, sfr::rockblock::normal_report.size());
}

int test_normal_report_acknowledge()
{
    UNITY_BEGIN();
    RUN_TEST(test_normal_report_without_commands);
    RUN_TEST(test_normal_report_with_one_command);
    RUN_TEST(test_normal_report_with_multiple_commands);
    RUN_TEST(test_normal_report_with_more_than_15_commands);
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
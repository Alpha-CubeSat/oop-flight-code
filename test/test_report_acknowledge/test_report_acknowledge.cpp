#include "Monitors/CommandMonitor.hpp"
#include "Monitors/NormalReportMonitor.hpp"
#include "RockblockSimulator.hpp"
#include "sfr.hpp"
#include <unity.h>

void test_normal_report_without_commands()
{
    NormalReportMonitor normal_report_monitor(0);
    normal_report_monitor.execute();
    TEST_ASSERT_EQUAL(37, sfr::rockblock::report.size());
}

void test_normal_report_with_one_command()
{
    NormalReportMonitor normal_report_monitor(0);
    CommandMonitor command_monitor(0);
    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::deployment);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    normal_report_monitor.execute();
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[0], sfr::rockblock::report[36]);
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[1], sfr::rockblock::report[37]);
    TEST_ASSERT_EQUAL(39, sfr::rockblock::report.size());
}

void test_normal_report_with_multiple_commands()
{
    NormalReportMonitor normal_report_monitor(0);
    CommandMonitor command_monitor(0);
    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::deployment);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_arm);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    normal_report_monitor.execute();
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[0], sfr::rockblock::report[36]);
    TEST_ASSERT_EQUAL(constants::rockblock::mission_mode[1], sfr::rockblock::report[37]);
    TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[0], sfr::rockblock::report[38]);
    TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[1], sfr::rockblock::report[39]);
    TEST_ASSERT_EQUAL(41, sfr::rockblock::report.size());
}

void test_normal_report_with_more_than_15_commands()
{
    NormalReportMonitor normal_report_monitor(0);
    CommandMonitor command_monitor(0);
    int i = 0;
    while (i < 30) {
        sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_arm);
        sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
        sfr::rockblock::waiting_command = true;
        command_monitor.execute();
        ++i;
    }
    normal_report_monitor.execute();
    i = 0;
    while (i < 15) {
        TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[0], sfr::rockblock::report[36 + (i * 2)]);
        TEST_ASSERT_EQUAL(constants::rockblock::burnwire_arm[1], sfr::rockblock::report[36 + (i * 2) + 1]);
        ++i;
    }
    TEST_ASSERT_EQUAL(constants::rockblock::end_of_normal_downlink_flag, sfr::rockblock::report[66]);
    TEST_ASSERT_EQUAL(67, sfr::rockblock::report.size());
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
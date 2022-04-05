#include "Monitors/RockblockReportMonitor.hpp"
#include <unity.h>

void test_rockblock_report_monitor_schedule()
{
    RockblockReportMonitor rockblock_report_monitor(0);
    // Testing for Rockblock Report Type Switching
    sfr::rockblock::mode = rockblock_mode_type::standby;

    sfr::imu::imu_dlink_report_ready = true;
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::imu_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == true);

    sfr::imu::imu_dlink_report_ready = false;
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::normal_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == true);

    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::normal_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == true);

    sfr::camera::report_ready = true;
    TEST_ASSERT_EQUAL(report_type::normal_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == true);

    // Testing for Rockblock Report Write
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::imu_report[i] = 1;
    }
    sfr::imu::imu_dlink_report_ready = true;
    rockblock_report_monitor.execute();
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::downlink_report[i], sfr::rockblock::imu_report[i]);
    }

    sfr::rockblock::normal_report.clear();
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::normal_report.push_back(2);
    }
    sfr::imu::imu_dlink_report_ready = false;
    rockblock_report_monitor.execute();
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::downlink_report[i], sfr::rockblock::normal_report[i]);
    }
    TEST_ASSERT_EQUAL(sfr::rockblock::downlink_report.size(), sfr::rockblock::normal_report.size());
}

int test_rockblock_report_monitor()
{
    UNITY_BEGIN();
    RUN_TEST(test_rockblock_report_monitor_schedule);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_rockblock_report_monitor();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    Serial.println("Rockblock Report Monitor Started");
    test_rockblock_report_monitor();
}

void loop() {}
#endif
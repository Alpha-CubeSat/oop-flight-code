#include "Monitors/RockblockReportMonitor.hpp"
#include <unity.h>

// Sets up Rockblock environment for testing
void set_rockblock_test_environment(report_type starting_report_type, bool is_imu_report_ready, bool is_camera_report_ready, int downlink_period)
{
    sfr::rockblock::downlink_report_type = starting_report_type;
    sfr::imu::report_ready = is_imu_report_ready;
    sfr::camera::report_ready = is_camera_report_ready;
    sfr::rockblock::downlink_period = downlink_period;
    sfr::rockblock::normal_report.clear();
    sfr::rockblock::imu_report.clear();
    sfr::rockblock::camera_report.clear();
    sfr::rockblock::downlink_report.clear();
}

// Tests for Rockblock Report Type Switching from Normal Report and Report Write
void test_schedule_from_normal_report()
{
    RockblockReportMonitor rockblock_report_monitor(0);
    sfr::rockblock::mode = rockblock_mode_type::standby;

    // IMU and Camera both available
    set_rockblock_test_environment(report_type::normal_report, true, true, 0);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::imu_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::imu_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == true);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::imu_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::imu_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Only IMU available
    set_rockblock_test_environment(report_type::normal_report, true, false, 0);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::imu_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::imu_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == true);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::imu_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::imu_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Both IMU and Camera unavailable
    set_rockblock_test_environment(report_type::normal_report, false, false, 0);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::normal_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::normal_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == true);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::normal_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::normal_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Both IMU and Camera unavailable, long downlink period
    set_rockblock_test_environment(report_type::normal_report, false, false, INT_MAX);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::normal_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == false);
}

int test_rockblock_report_monitor()
{
    UNITY_BEGIN();
    RUN_TEST(test_schedule_from_normal_report);
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
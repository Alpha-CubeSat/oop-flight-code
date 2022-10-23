#include "Monitors/RockblockReportMonitor.hpp"
#include <unity.h>

// Sets up Rockblock environment for testing
void set_rockblock_test_environment(report_type starting_report_type, bool is_imu_report_ready, bool is_camera_report_ready, int downlink_period)
{
    sfr::rockblock::downlink_report_type = (uint16_t)starting_report_type;
    sfr::imu::report_ready = is_imu_report_ready;
    sfr::camera::report_ready = is_camera_report_ready;
    sfr::rockblock::downlink_period = downlink_period;
    sfr::rockblock::normal_report.clear();
    sfr::rockblock::imu_report.clear();
    sfr::rockblock::camera_report.clear();
    sfr::rockblock::downlink_report.clear();
}

// Tests for Rockblock Report Type Switching from Normal Report
void test_schedule_from_normal_report()
{
    RockblockReportMonitor rockblock_report_monitor(0);
    sfr::rockblock::mode = (uint16_t)rockblock_mode_type::standby;

    // IMU and Camera both available
    set_rockblock_test_environment(report_type::normal_report, true, true, 0);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::imu_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::imu_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status);

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
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::imu_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::imu_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Both IMU and Camera unavailable, time since last downlink exceeds downlink period
    set_rockblock_test_environment(report_type::normal_report, false, false, 0);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::normal_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::normal_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::normal_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::normal_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Both IMU and Camera unavailable, downlink period exceeds time since last downlink
    set_rockblock_test_environment(report_type::normal_report, false, false, INT_MAX);
    rockblock_report_monitor.execute();
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == false);
}

// Tests for Rockblock Report Type Switching from IMU Report
void test_schedule_from_imu_report()
{
    RockblockReportMonitor rockblock_report_monitor(0);
    sfr::rockblock::mode = (uint16_t)rockblock_mode_type::standby;

    // Normal and Camera both available
    set_rockblock_test_environment(report_type::imu_report, true, true, 0);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::camera_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::camera_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::camera_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::camera_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Only IMU available and time since last downlink exceeds downlink period
    set_rockblock_test_environment(report_type::imu_report, true, false, 0);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::normal_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::normal_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::normal_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::normal_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Only IMU available and downlink period exceeds time since last downlink
    set_rockblock_test_environment(report_type::imu_report, true, false, INT_MAX);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::imu_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::imu_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::imu_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::imu_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Both IMU and Camera unavailable, downlink period exceeds time since last downlink
    set_rockblock_test_environment(report_type::imu_report, false, false, INT_MAX);
    rockblock_report_monitor.execute();
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == false);
}

// Tests for Rockblock Report Type Switching from Camera Report
void test_schedule_from_camera_report()
{
    RockblockReportMonitor rockblock_report_monitor(0);
    sfr::rockblock::mode = (uint16_t)rockblock_mode_type::standby;

    // Normal and Camera both available
    set_rockblock_test_environment(report_type::camera_report, true, true, 0);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::normal_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::normal_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::normal_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::normal_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Both IMU and Camera available and downlink period exceeds time since last downlink
    set_rockblock_test_environment(report_type::camera_report, true, true, INT_MAX);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::imu_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::imu_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::imu_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::imu_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Only Camera available and downlink period exceeds time since last downlink
    set_rockblock_test_environment(report_type::camera_report, false, true, INT_MAX);
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::camera_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::camera_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::camera_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::camera_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Both IMU and Camera unavailable, downlink period exceeds time since last downlink
    set_rockblock_test_environment(report_type::camera_report, false, false, INT_MAX);
    rockblock_report_monitor.execute();
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == false);
}

// void test_rockblock_checksum()
// {
//     RockblockReportMonitor rockblock_report_monitor(0);
//     sfr::rockblock::mode = (uint16_t)rockblock_mode_type::standby;

//     // IMU and Camera both available
//     set_rockblock_test_environment(report_type::normal_report, true, true, 0);
//     for (int i = 0; i < constants::rockblock::packet_size; i++) {
//         sfr::rockblock::normal_report.push_back(1);
//     }
//     rockblock_report_monitor.execute();
//     TEST_ASSERT_EQUAL(report_type::normal_report, sfr::rockblock::downlink_report_type);
//     TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status);

//     TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::normal_report.size());
//     TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
//     for (int i = 0; i < constants::rockblock::packet_size; i++) {
//         TEST_ASSERT_EQUAL(sfr::rockblock::normal_report[i], sfr::rockblock::downlink_report[i]);
//     }
//     TEST_ASSERT_EQUAL(0, 1);
// }

void test_rockblock_report_monitor_low_power_mode()
{
    RockblockReportMonitor rockblock_report_monitor(0);
    sfr::rockblock::mode = (uint16_t)rockblock_mode_type::standby;

    // Normal and Camera both available, low downlink period
    set_rockblock_test_environment(report_type::camera_report, true, true, 0);
    // In low power deployment mode
    sfr::mission::current_mode = sfr::mission::lowPowerDeployment;
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        sfr::rockblock::normal_report.push_back(1);
    }
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::normal_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status);

    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::normal_report.size());
    TEST_ASSERT_EQUAL(constants::rockblock::packet_size, sfr::rockblock::downlink_report.size());
    for (int i = 0; i < constants::rockblock::packet_size; i++) {
        TEST_ASSERT_EQUAL(sfr::rockblock::normal_report[i], sfr::rockblock::downlink_report[i]);
    }

    // Normal and Camera both available, high downlink period
    set_rockblock_test_environment(report_type::camera_report, true, true, INT_MAX);
    // In low power deployment mode
    sfr::mission::current_mode = sfr::mission::lowPowerDeployment;
    rockblock_report_monitor.execute();
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == false);
}

int test_rockblock_report_monitor()
{
    UNITY_BEGIN();
    RUN_TEST(test_schedule_from_normal_report);
    RUN_TEST(test_schedule_from_imu_report);
    RUN_TEST(test_schedule_from_camera_report);
    RUN_TEST(test_rockblock_report_monitor_low_power_mode);
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
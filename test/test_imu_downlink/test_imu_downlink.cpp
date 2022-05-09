#include "Monitors/IMUDownlink.hpp"
#include "Monitors/IMUDownlinkReportMonitor.hpp"
#include <unity.h>

void buffer_reset_default()
{
    sfr::imu::sample_gyro = false;
    sfr::mission::time_deployed = 0.0;
    sfr::imu::gyro_x = 0;
    sfr::imu::gyro_y = 0;
    sfr::imu::gyro_z = 0;
    // make sure the buffer size is starting from 0
    while (sfr::imu::imu_dlink.size() != 0) {
        sfr::imu::imu_dlink.pop_back();
    }
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    sfr::imu::gyro_x_value->set_value(sfr::imu::gyro_x);
    sfr::imu::gyro_y_value->set_value(sfr::imu::gyro_y);
    sfr::imu::gyro_z_value->set_value(sfr::imu::gyro_z);
}

void test_imu_downlink_mandburn()
{
    buffer_reset_default();
    sfr::mission::current_mode = sfr::mission::mandatoryBurns;
    IMUDownlink imu_downlink(0);
    imu_downlink.execute();
    TEST_ASSERT_EQUAL(3, sfr::imu::imu_dlink.size());
    uint8_t gyro_x_mapped = map(sfr::imu::gyro_x, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    uint8_t gyro_y_mapped = map(sfr::imu::gyro_y, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    uint8_t gyro_z_mapped = map(sfr::imu::gyro_z, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    uint8_t gyro_x_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    uint8_t gyro_y_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    uint8_t gyro_z_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    TEST_ASSERT_EQUAL(gyro_x_popped, gyro_x_mapped);
    TEST_ASSERT_EQUAL(gyro_y_popped, gyro_y_mapped);
    TEST_ASSERT_EQUAL(gyro_z_popped, gyro_z_mapped);
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    sfr::imu::sample_gyro = false;
}

void test_imu_downlink_regburn()
{
    buffer_reset_default();
    sfr::mission::current_mode = sfr::mission::regularBurns;
    IMUDownlink imu_downlink(0);
    imu_downlink.execute();
    TEST_ASSERT_EQUAL(3, sfr::imu::imu_dlink.size());
    uint8_t gyro_x_mapped = map(sfr::imu::gyro_x, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    uint8_t gyro_y_mapped = map(sfr::imu::gyro_y, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    uint8_t gyro_z_mapped = map(sfr::imu::gyro_z, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    uint8_t gyro_x_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    uint8_t gyro_y_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    uint8_t gyro_z_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    TEST_ASSERT_EQUAL(gyro_x_popped, gyro_x_mapped);
    TEST_ASSERT_EQUAL(gyro_y_popped, gyro_y_mapped);
    TEST_ASSERT_EQUAL(gyro_z_popped, gyro_z_mapped);
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    sfr::imu::sample_gyro = false;
}
void test_imu_downlink_deployed()
{
    buffer_reset_default();
    sfr::mission::current_mode = sfr::mission::photo;
    sfr::mission::deployed = true;
    IMUDownlink imu_downlink(0);
    imu_downlink.execute();
    TEST_ASSERT_EQUAL(3, sfr::imu::imu_dlink.size());
    uint8_t gyro_x_mapped = map(sfr::imu::gyro_x, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    uint8_t gyro_y_mapped = map(sfr::imu::gyro_y, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    uint8_t gyro_z_mapped = map(sfr::imu::gyro_z, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    uint8_t gyro_x_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    uint8_t gyro_y_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    uint8_t gyro_z_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    TEST_ASSERT_EQUAL(gyro_x_popped, gyro_x_mapped);
    TEST_ASSERT_EQUAL(gyro_y_popped, gyro_y_mapped);
    TEST_ASSERT_EQUAL(gyro_z_popped, gyro_z_mapped);
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    sfr::imu::sample_gyro = false;
}

void write_to_report()
{
    buffer_reset_default();
    uint8_t gyro_x_mapped = map(sfr::imu::gyro_x, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    uint8_t gyro_y_mapped = map(sfr::imu::gyro_y, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    uint8_t gyro_z_mapped = map(sfr::imu::gyro_z, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
    for (int i = 0; i < 22; i++) {
        sfr::imu::imu_dlink.push_front(gyro_x_mapped);
        sfr::imu::imu_dlink.push_front(gyro_y_mapped);
        sfr::imu::imu_dlink.push_front(gyro_z_mapped);
    }
}
void test_imu_report()
{
    write_to_report();
    IMUDownlinkReportMonitor imu_report_monitor(0);
    sfr::imu::report_ready = true;
    sfr::rockblock::downlink_report_type = report_type::imu_report;
    sfr::imu::report_downlinked = true;
    sfr::imu::report_written = true;
    sfr::imu::fragment_number = 1;
    imu_report_monitor.execute();
    TEST_ASSERT_EQUAL(70, sfr::rockblock::imu_report.size());
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    TEST_ASSERT_EQUAL(false, sfr::imu::report_ready);
    TEST_ASSERT_EQUAL(false, sfr::imu::report_downlinked);
    TEST_ASSERT_EQUAL(1, sfr::imu::fragment_number);
}

int test_rockblock_report_monitor()
{
    UNITY_BEGIN();
    RUN_TEST(test_imu_downlink_mandburn);
    RUN_TEST(test_imu_downlink_regburn);
    RUN_TEST(test_imu_downlink_deployed);
    RUN_TEST(test_imu_report);
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
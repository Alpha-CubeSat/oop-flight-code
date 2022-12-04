#include "Monitors/IMUDownlink.hpp"
#include "Monitors/IMUDownlinkReportMonitor.hpp"
#include <unity.h>

void buffer_reset_default()
{
    sfr::imu::sample_gyro = false;
    sfr::imu::report_written = false;
    sfr::mission::time_deployed = 0.0;
    sfr::imu::gyro_x_average = 0;
    sfr::imu::gyro_y_average = 0;
    sfr::imu::gyro_z_average = 0;
    // make sure the buffer size is starting from 0
    sfr::imu::imu_dlink.clear();
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    sfr::imu::gyro_x_value->set_value(0);
    sfr::imu::gyro_y_value->set_value(0);
    sfr::imu::gyro_z_value->set_value(0);
}

void test_imu_downlink_mandburn()
{
    TEST_ASSERT_FALSE(sfr::imu::report_ready);
    buffer_reset_default();
    sfr::mission::current_mode = sfr::mission::mandatoryBurns;
    IMUDownlink imu_downlink(0);
    imu_downlink.execute();
    TEST_ASSERT_EQUAL(true, sfr::imu::sample_gyro);
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    imu_downlink.execute();
    TEST_ASSERT_EQUAL(3, sfr::imu::imu_dlink.size());
    uint8_t gyro_x_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    uint8_t gyro_y_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    uint8_t gyro_z_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    TEST_ASSERT_EQUAL(gyro_x_popped, 125); // 0.0 is converted to 125 int
    TEST_ASSERT_EQUAL(gyro_y_popped, 125);
    TEST_ASSERT_EQUAL(gyro_z_popped, 125);
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    sfr::imu::sample_gyro = false;
}

void test_imu_downlink_regburn()
{
    buffer_reset_default();
    sfr::mission::current_mode = sfr::mission::regularBurns;
    IMUDownlink imu_downlink(0);
    imu_downlink.execute();
    TEST_ASSERT_EQUAL(true, sfr::imu::sample_gyro);
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    imu_downlink.execute();
    TEST_ASSERT_EQUAL(3, sfr::imu::imu_dlink.size());
    uint8_t gyro_x_exp = 125;
    uint8_t gyro_y_exp = 125;
    uint8_t gyro_z_exp = 125;
    uint8_t gyro_x_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    uint8_t gyro_y_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    uint8_t gyro_z_popped = sfr::imu::imu_dlink.back();
    sfr::imu::imu_dlink.pop_back();
    TEST_ASSERT_EQUAL(gyro_x_popped, gyro_x_exp);
    TEST_ASSERT_EQUAL(gyro_y_popped, gyro_y_exp);
    TEST_ASSERT_EQUAL(gyro_z_popped, gyro_z_exp);
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
    TEST_ASSERT_FALSE(sfr::imu::report_written);
    TEST_ASSERT_EQUAL(true, sfr::imu::sample_gyro);
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    imu_downlink.execute();
    TEST_ASSERT_EQUAL(3, sfr::imu::imu_dlink.size());
    uint8_t gyro_x_mapped = 125;
    uint8_t gyro_y_mapped = 125;
    uint8_t gyro_z_mapped = 125;
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

void test_imu_downlink_delay()
{
    buffer_reset_default();
    sfr::mission::current_mode = sfr::mission::photo;
    sfr::mission::deployed = true;
    IMUDownlink imu_downlink(0);
    imu_downlink.execute();
    TEST_ASSERT_FALSE(sfr::imu::report_written);
    TEST_ASSERT_EQUAL(true, sfr::imu::sample_gyro);
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    delay(60 * constants::time::one_second);
    imu_downlink.execute();
    TEST_ASSERT_TRUE(sfr::imu::report_written);
    TEST_ASSERT_FALSE(sfr::imu::sample_gyro);
    TEST_ASSERT_EQUAL(3, sfr::imu::imu_dlink.size());
}

void write_to_report(int val)
{
    buffer_reset_default();
    uint8_t gyro_x_mapped = val;
    uint8_t gyro_y_mapped = val;
    uint8_t gyro_z_mapped = val;
    for (int i = 0; i < 22; i++) {
        sfr::imu::imu_dlink.push_front(gyro_x_mapped);
        sfr::imu::imu_dlink.push_front(gyro_y_mapped);
        sfr::imu::imu_dlink.push_front(gyro_z_mapped);
    }
}
void test_imu_report()
{
    int val = 0.0;
    write_to_report((int)((val+5)*25));
    IMUDownlinkReportMonitor imu_report_monitor(0);
    sfr::imu::report_ready = true;
    sfr::rockblock::downlink_report_type = (uint16_t)report_type::imu_report;
    sfr::imu::report_written = true;
    imu_report_monitor.execute();
    TEST_ASSERT_EQUAL(68, sfr::rockblock::imu_report.size());
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    TEST_ASSERT_TRUE(sfr::imu::report_ready);
    TEST_ASSERT_FALSE(sfr::rockblock::imu_report.empty());
    TEST_ASSERT_EQUAL(24, sfr::rockblock::imu_report.front());
    sfr::rockblock::imu_report.pop_front();
    TEST_ASSERT_EQUAL(0, sfr::rockblock::imu_report.front());
    sfr::rockblock::imu_report.push_front(24);
    TEST_ASSERT_EQUAL((int)((val+5)*25), sfr::rockblock::imu_report.back());
    sfr::rockblock::imu_report.clear();
    val = 1;
    write_to_report((int)((val+5)*25));
    sfr::imu::report_written = true;
    imu_report_monitor.execute();
    TEST_ASSERT_EQUAL(68, sfr::rockblock::imu_report.size());
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    TEST_ASSERT_TRUE(sfr::imu::report_ready);
    TEST_ASSERT_FALSE(sfr::rockblock::imu_report.empty());
    TEST_ASSERT_EQUAL((int)((val+5)*25), sfr::rockblock::imu_report.back());
    sfr::rockblock::imu_report.clear();
    val = -5;
    write_to_report((int)((val+5)*25));
    sfr::imu::report_written = true;
    imu_report_monitor.execute();
    TEST_ASSERT_EQUAL(68, sfr::rockblock::imu_report.size());
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    TEST_ASSERT_TRUE(sfr::imu::report_ready);
    TEST_ASSERT_FALSE(sfr::rockblock::imu_report.empty());
    TEST_ASSERT_EQUAL((int)((val+5)*25), sfr::rockblock::imu_report.back());
    sfr::rockblock::imu_report.clear();
    val = 5;
    write_to_report((int)((val+5)*25));
    TEST_ASSERT_EQUAL(66, sfr::imu::imu_dlink.size());
    sfr::imu::report_written = true;
    imu_report_monitor.execute();
    TEST_ASSERT_EQUAL(68, sfr::rockblock::imu_report.size());
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink.size());
    TEST_ASSERT_TRUE(sfr::imu::report_ready);
    TEST_ASSERT_FALSE(sfr::rockblock::imu_report.empty());
    TEST_ASSERT_EQUAL((int)((val+5)*25), sfr::rockblock::imu_report.back());
    sfr::rockblock::imu_report.clear();
    sfr::imu::imu_dlink.clear();
    TEST_ASSERT_TRUE(sfr::imu::report_written);
    imu_report_monitor.execute();
    TEST_ASSERT_FALSE(sfr::imu::report_ready);

}

int test_imu_downlink()
{
    UNITY_BEGIN();
    RUN_TEST(test_imu_downlink_mandburn);
    RUN_TEST(test_imu_downlink_regburn);
    RUN_TEST(test_imu_downlink_deployed);
    RUN_TEST(test_imu_downlink_delay); // Delays a minute
    RUN_TEST(test_imu_report);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_imu_downlink();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    Serial.println("IMU Downlink Started");
    test_imu_downlink();
}

void loop() {}
#endif
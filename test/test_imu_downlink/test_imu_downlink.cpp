#include "Monitors/IMUDownlink.hpp"
#include "Monitors/IMUDownlinkReportMonitor.hpp"
#include <unity.h>

void buffer_size_back_to_zero()
{
    sfr::imu::gyro_x = 64;
    sfr::imu::gyro_y = 65;
    sfr::imu::gyro_z = 66;
    // make sure the three buffer sizes are starting from 0
    while (sfr::imu::imu_dlink_gyro_x_buffer.size() != 0) {
        sfr::imu::imu_dlink_gyro_x_buffer.pop_back();
    }
    while (sfr::imu::imu_dlink_gyro_y_buffer.size() != 0) {
        sfr::imu::imu_dlink_gyro_y_buffer.pop_back();
    }
    while (sfr::imu::imu_dlink_gyro_z_buffer.size() != 0) {
        sfr::imu::imu_dlink_gyro_z_buffer.pop_back();
    }
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink_gyro_x_buffer.size());
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink_gyro_y_buffer.size());
    TEST_ASSERT_EQUAL(0, sfr::imu::imu_dlink_gyro_z_buffer.size());
}

void test_IMUDownlink_execute_mandburns()
{
    buffer_size_back_to_zero();
    sfr::mission::current_mode == sfr::mission::mandatoryBurns;
    TEST_ASSERT_EQUAL(sfr::mission::current_mode, sfr::mission::mandatoryBurns);
    IMUDownlink imu_downlink(0);
    imu_downlink.execute();

    // test for mission mode is mand_burn

    TEST_ASSERT_EQUAL(1, sfr::imu::imu_dlink_gyro_z_buffer.size());
    TEST_ASSERT_EQUAL(1, sfr::imu::imu_dlink_gyro_y_buffer.size());
    TEST_ASSERT_EQUAL(1, sfr::imu::imu_dlink_gyro_z_buffer.size());
}

void test_IMUDownlink_execute_regburns()
{
    buffer_size_back_to_zero();
    sfr::mission::current_mode == sfr::mission::regularBurns;
    IMUDownlink imu_downlink(0);
    imu_downlink.execute();

    // test for mission mode is mand_burn
    TEST_ASSERT_EQUAL(1, sfr::imu::imu_dlink_gyro_x_buffer.size());
    TEST_ASSERT_EQUAL(1, sfr::imu::imu_dlink_gyro_y_buffer.size());
    TEST_ASSERT_EQUAL(1, sfr::imu::imu_dlink_gyro_z_buffer.size());
}

void test_IMUDownlink_buffer_size()
{

    buffer_size_back_to_zero();
    // reach the max size of the IMU downlink buffer
    for (int i = 0; i < 3; i++) {
        sfr::imu::imu_dlink_gyro_x_buffer.push_front(sfr::imu::gyro_x);
        sfr::imu::imu_dlink_gyro_y_buffer.push_front(sfr::imu::gyro_y);
        sfr::imu::imu_dlink_gyro_z_buffer.push_front(sfr::imu::gyro_z);
    }
    sfr::mission::current_mode == sfr::mission::regularBurns;
    TEST_ASSERT_EQUAL(sfr::mission::current_mode, sfr::mission::regularBurns);
    IMUDownlink imu_downlink(0);
    imu_downlink.execute();
    TEST_ASSERT_EQUAL(sfr::imu::imu_downlink_buffer_max_size, sfr::imu::imu_dlink_gyro_x_buffer.size());
    TEST_ASSERT_EQUAL(sfr::imu::imu_downlink_buffer_max_size, sfr::imu::imu_dlink_gyro_y_buffer.size());
    TEST_ASSERT_EQUAL(sfr::imu::imu_downlink_buffer_max_size, sfr::imu::imu_dlink_gyro_z_buffer.size());

    // starting as exceeded buffer size
    sfr::imu::imu_dlink_gyro_x_buffer.push_front(sfr::imu::gyro_x);
    sfr::imu::imu_dlink_gyro_y_buffer.push_front(sfr::imu::gyro_y);
    sfr::imu::imu_dlink_gyro_z_buffer.push_front(sfr::imu::gyro_z);
    imu_downlink.execute();
    TEST_ASSERT_EQUAL(sfr::imu::imu_downlink_buffer_max_size, sfr::imu::imu_dlink_gyro_x_buffer.size());
    TEST_ASSERT_EQUAL(sfr::imu::imu_downlink_buffer_max_size, sfr::imu::imu_dlink_gyro_y_buffer.size());
    TEST_ASSERT_EQUAL(sfr::imu::imu_downlink_buffer_max_size, sfr::imu::imu_dlink_gyro_z_buffer.size());
}

void test_IMUDownlink_report_execute()
{
    sfr::imu::gyro_x = 64;
    sfr::imu::gyro_y = 65;
    sfr::imu::gyro_z = 66;
    uint8_t gyro_x = map(sfr::imu::gyro_x, constants::imu::min_gyro_x, constants::imu::max_gyro_x, 0, 255);
    uint8_t gyro_y = map(sfr::imu::gyro_y, constants::imu::min_gyro_y, constants::imu::max_gyro_y, 0, 255);
    uint8_t gyro_z = map(sfr::imu::gyro_z, constants::imu::min_gyro_z, constants::imu::max_gyro_z, 0, 255);

    for (int i = 0; i < 3; i++) {
        sfr::imu::imu_dlink_gyro_x_buffer.push_front(sfr::imu::gyro_x);
        sfr::imu::imu_dlink_gyro_y_buffer.push_front(sfr::imu::gyro_y);
        sfr::imu::imu_dlink_gyro_z_buffer.push_front(sfr::imu::gyro_z);
    }
    // test when fragment number has not exceeded the limit
    sfr::imu::report_downlinked = true;
    sfr::imu::fragment_requested = true;
    sfr::imu::fragment_number = 7;
    IMUDownlinkReportMonitor imu_downlink_report(0);

    imu_downlink_report.execute();

    TEST_ASSERT_EQUAL(false, sfr::imu::fragment_requested);
    TEST_ASSERT_EQUAL(8, sfr::imu::fragment_number);
    TEST_ASSERT_EQUAL(70, sizeof(sfr::rockblock::imu_report));
    TEST_ASSERT_EQUAL(true, sfr::imu::imu_dlink_report_ready);
    TEST_ASSERT_EQUAL(false, sfr::imu::report_downlinked);
    TEST_ASSERT_EQUAL(88, sfr::rockblock::imu_report[0]);
    TEST_ASSERT_EQUAL(7, sfr::rockblock::imu_report[1]);
    for (int i = 2; i < 2 + sfr::imu::imu_dlink_gyro_x_buffer.size() * 3; i += 3) {
        TEST_ASSERT_EQUAL(gyro_x, sfr::rockblock::imu_report[i]);
        TEST_ASSERT_EQUAL(gyro_y, sfr::rockblock::imu_report[i + 1]);
        TEST_ASSERT_EQUAL(gyro_z, sfr::rockblock::imu_report[i + 2]);
    }

    // test when fragment number has exceeded the limit
    sfr::imu::report_downlinked = true;
    sfr::imu::fragment_requested = true;
    sfr::imu::fragment_number = sfr::rockblock::imu_max_fragments - 1;

    imu_downlink_report.execute();

    TEST_ASSERT_EQUAL(false, sfr::imu::fragment_requested);
    TEST_ASSERT_EQUAL(0, sfr::imu::fragment_number);
    TEST_ASSERT_EQUAL(70, sizeof(sfr::rockblock::imu_report));
    TEST_ASSERT_EQUAL(false, sfr::imu::imu_dlink_report_ready);
    TEST_ASSERT_EQUAL(false, sfr::imu::report_downlinked);
    TEST_ASSERT_EQUAL(88, sfr::rockblock::imu_report[0]);
    TEST_ASSERT_EQUAL(sfr::rockblock::imu_max_fragments - 1, sfr::rockblock::imu_report[1]);
    for (int i = 2; i < 2 + sfr::imu::imu_dlink_gyro_x_buffer.size() * 3; i += 3) {
        TEST_ASSERT_EQUAL(gyro_x, sfr::rockblock::imu_report[i]);
        TEST_ASSERT_EQUAL(gyro_y, sfr::rockblock::imu_report[i + 1]);
        TEST_ASSERT_EQUAL(gyro_z, sfr::rockblock::imu_report[i + 2]);
    }
}

void test_create_imu_downlink_report()
{
}

int test_imu_downlink()
{
    UNITY_BEGIN();
    RUN_TEST(test_IMUDownlink_execute_mandburns);
    RUN_TEST(test_IMUDownlink_execute_regburns);
    RUN_TEST(test_IMUDownlink_buffer_size);
    RUN_TEST(test_IMUDownlink_report_execute);
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
    test_imu_downlink();
}

void loop() {}
#endif
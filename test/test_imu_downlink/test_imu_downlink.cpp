#include "Monitors/IMUDownlink.hpp"
#include "Monitors/IMUDownlinkReportMonitor.hpp"
#include <unity.h>

void test_dummy_gyro()
{
    IMUDownlinkReportMonitor imu_downlink_report(0);
    imu_downlink_report.execute();
    uint8_t gyro_x = map(sfr::imu::gyro_x_average, constants::imu::min_gyro_x, constants::imu::max_gyro_x, 0, 255);
    uint8_t gyro_y = map(sfr::imu::gyro_y_average, constants::imu::min_gyro_y, constants::imu::max_gyro_y, 0, 255);
    uint8_t gyro_z = map(sfr::imu::gyro_z_average, constants::imu::min_gyro_z, constants::imu::max_gyro_z, 0, 255);
    TEST_ASSERT_EQUAL(sfr::imu::report[1], gyro_x);
    TEST_ASSERT_EQUAL(sfr::imu::report[2], gyro_y);
    TEST_ASSERT_EQUAL(sfr::imu::report[3], gyro_z);
}

int test_imu_downlink()
{
    UNITY_BEGIN();
    RUN_TEST(test_dummy_gyro);
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
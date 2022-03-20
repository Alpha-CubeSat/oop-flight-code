#include "Monitors/RockblockReportMonitor.hpp"
#include <unity.h>

void test_rockblock_report_monitor_schedule()
{
    RockblockReportMonitor rockblock_report_monitor(0);
    sfr::rockblock::mode = rockblock_mode_type::standby;
    sfr::imu::imu_dlink_report_ready = true;
    rockblock_report_monitor.execute();
    TEST_ASSERT_EQUAL(report_type::imu_report, sfr::rockblock::downlink_report_type);
    TEST_ASSERT_TRUE(sfr::rockblock::rockblock_ready_status == true);
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
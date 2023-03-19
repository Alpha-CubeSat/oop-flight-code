#include <Monitors/NormalReportMonitor.hpp>
#include <unity.h>
void test_serialize()
{
    NormalReportMonitor normal_report_monitor(0.);

    // test burn_time
    // test negative values
    normal_report_monitor.serialize(4, -50.0);
    normal_report_monitor.serialize(4, -1.1234567890);
    normal_report_monitor.serialize(4, -10000.0);
    // test 0
    normal_report_monitor.serialize(4, 0.);
    // test positive in bounds
    normal_report_monitor.serialize(4, 1 / 3);
    normal_report_monitor.serialize(4, 50.987654321);
    normal_report_monitor.serialize(4, 10000.0);

    // test armed_time
    normal_report_monitor.serialize(5, -50.0);
    normal_report_monitor.serialize(5, -1.1234567890);
    normal_report_monitor.serialize(5, -10000.0);
    // test 0
    normal_report_monitor.serialize(5, 0.);
    // test positive in bounds
    normal_report_monitor.serialize(5, 1 / 3);
    normal_report_monitor.serialize(5, 50.987654321);
    normal_report_monitor.serialize(5, 10000.0);

    // test downlink_period
    normal_report_monitor.serialize(8, -50.0);
    normal_report_monitor.serialize(8, -1.1234567890);
    normal_report_monitor.serialize(8, -10000.0);
    // test 0
    normal_report_monitor.serialize(8, 0.);
    // test positive in bounds
    normal_report_monitor.serialize(8, 1 / 3);
    normal_report_monitor.serialize(8, 50.987654321);
    normal_report_monitor.serialize(8, 10000.0);

    // test mag_x, mag_y, mag_z
    // test negative values
    normal_report_monitor.serialize(11, -50.0);
    normal_report_monitor.serialize(4, -1.1234567890);
    normal_report_monitor.serialize(11, -10000.);
    // test 0
    normal_report_monitor.serialize(11, 0.);
    // test positive in bounds
    normal_report_monitor.serialize(11, 50.987654321);
    normal_report_monitor.serialize(11, 1 / 3);
    // test positive value out of bounds
    normal_report_monitor.serialize(11, 10000.);

    // test gyro_x, gyro_y, gyro_z
    normal_report_monitor.serialize(14, -50.);
    normal_report_monitor.serialize(14, -1.1234567890);
    normal_report_monitor.serialize(14, -100000000.);
    // test 0
    normal_report_monitor.serialize(14, 0.);
    // test positive in bounds
    normal_report_monitor.serialize(14, 1 / 3);
    normal_report_monitor.serialize(14, 50.987654321);
    // test positive value out of bounds
    normal_report_monitor.serialize(14, 100000000.);

    // test photoresistor
    normal_report_monitor.serialize(17, -50.);
    normal_report_monitor.serialize(17, -1.1234567890);
    normal_report_monitor.serialize(17, -100000000.);
    // test 0
    normal_report_monitor.serialize(17, 0.);
    // test positive in bounds
    normal_report_monitor.serialize(17, 1 / 3);
    normal_report_monitor.serialize(17, 50.987654321);
    // test positive value out of bounds
    normal_report_monitor.serialize(17, 100000000.);

    // test temp
    normal_report_monitor.serialize(18, -5.);
    normal_report_monitor.serialize(18, -1.1234567890);
    normal_report_monitor.serialize(18, -100000000.0);
    // test 0
    normal_report_monitor.serialize(18, 0.);
    // test positive in bounds
    normal_report_monitor.serialize(18, 1 / 3);
    normal_report_monitor.serialize(18, 50.987654321);
    normal_report_monitor.serialize(18, 100000000.);
}

int test_out_of_bounds_values()
{
    UNITY_BEGIN();
    RUN_TEST(test_serialize);
    return UNITY_END();
}
#ifdef DESKTOP
int main()
{
    return test_out_of_bounds_values();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    test_out_of_bounds_values();
}

void loop() {}
#endif
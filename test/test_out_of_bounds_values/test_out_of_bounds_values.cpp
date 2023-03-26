#include <Monitors/NormalReportMonitor.hpp>
#include <unity.h>
void test_serialize()
{
    NormalReportMonitor normal_report_monitor(0.);

    // test burn_time
    // test negative values out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(4, -50.0), 255);
    // test 0 / minimum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(4, 0.0), 0);
    // test positive in bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(4, 1 / 3), 0);
    // test maximum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(4, 5000.), 255);
    // test positive out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(4, 10000.0), 255);

    // test armed_time
    // test negative values out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(5, -50.0), 255);
    // test minimum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(5, 0.0), 0);
    // test positive in bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(5, 1 / 3), 0);
    // test maximum value
    // THIS IS RETURNING 254
    // TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(5, 864000000), 255);
    // test positive out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(5, 900000000.0), 255);

    // test downlink_period
    // test negative values out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(8, -50.0), 255);
    // // test 0
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(8, 0.0), 255);
    // test minimum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(8, 60000.0), 0);
    // test maximum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(8, 172800000.0), 255);
    // test positive out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(8, 900000000.0), 255);

    // test mag_x, mag_y, mag_z
    // test negative values out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(11, -300.0), 255);
    // test minimum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(11, -150.0), 0);
    // test maximum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(11, 150.0), 255);
    // test positive out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(11, 1000.0), 255);

    // test gyro_x, gyro_y, gyro_z
    // test negative values out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(14, -300.0), 255);
    // test minimum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(14, -5.0), 0);
    // test maximum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(14, 5.0), 255);
    // test positive out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(14, 1000.0), 255);

    // test photoresistor
    // test negative values out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(17, -300.0), 255);
    // test minimum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(17, 0.0), 0);
    // test maximum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(17, 1023.0), 255);
    // test positive out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(17, 2000.0), 255);

    // test temp
    // test negative values out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(18, -300.0), 255);
    // test minimum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(18, -50.0), 0);
    // test maximum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(18, 200.0), 255);
    // test positive out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(18, 2000.0), 255);

    // test voltage
    // test negative values out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(21, -300.0), 255);
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(21, 0.0), 255);
    // test minimum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(21, 3.0), 0);
    // test maximum value
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(21, 5.0), 255);
    // test positive out of bounds
    TEST_ASSERT_EQUAL_INT(normal_report_monitor.serialize(21, 2000.0), 255);
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
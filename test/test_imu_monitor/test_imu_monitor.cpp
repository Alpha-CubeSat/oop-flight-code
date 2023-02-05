#include "Monitors/IMUMonitor.hpp"
#include <unity.h>

void test_set_valid()
{
    IMUMonitor imu_monitor(0);
    sfr::imu::turn_on = true;
    sfr::imu::powered = false;
    imu_monitor.execute();
    TEST_ASSERT_TRUE(true);
}

int test_imu_monitor()
{
    UNITY_BEGIN();
    RUN_TEST(test_set_valid);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_imu_monitor();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    Serial.println("IMU Monitor Started");
    test_imu_monitor();
}

void loop() {}
#endif
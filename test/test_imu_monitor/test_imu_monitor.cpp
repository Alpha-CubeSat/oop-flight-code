#include "Monitors/IMUMonitor.hpp"
#include <unity.h>

void test_set_valid()
{
    IMUMonitor imu_monitor(0);
    sfr::imu::turn_on = true;
    sfr::imu::powered = false;
    imu_monitor.execute();
}

void test_imu_power_cycle()
{
    IMUMonitor imu_monitor(0);
    TEST_ASSERT_FALSE(sfr::imu::powered);
    imu_monitor.execute();
    TEST_ASSERT_TRUE(sfr::imu::powered);
    TEST_ASSERT_FALSE(sfr::imu::turn_on);
    TEST_ASSERT_TRUE(sfr::imu::mag_x_average->is_valid());

    sfr::imu::turn_off = true;
    imu_monitor.execute();
    TEST_ASSERT_FALSE(Pins::getPinState(constants::imu::CSAG));
    TEST_ASSERT_FALSE(Pins::getPinState(constants::imu::CSM));
    TEST_ASSERT_FALSE(sfr::imu::powered);
    TEST_ASSERT_FALSE(sfr::imu::turn_off);
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
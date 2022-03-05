#include "Monitors/FaultMonitor.hpp"
#include <MissionManager.hpp>
#include <unity.h>
void test_valid_initialization()
{
    MissionManager mission_manager(0);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::boot->id(), sfr::mission::current_mode->id());
}

void test_exit_boot()
{
    MissionManager mission_manager(0);

    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::boot->id(), sfr::mission::current_mode->id());

    sfr::mission::max_boot_time = 500;
    delay(sfr::mission::max_boot_time);
    mission_manager.execute();
    Serial.println(sfr::mission::current_mode->id());
    TEST_ASSERT_EQUAL(sfr::mission::aliveSignal->id(), sfr::mission::current_mode->id());
}

int test_mission_manager()
{
    UNITY_BEGIN();
    RUN_TEST(test_valid_initialization);
    RUN_TEST(test_exit_boot);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_mission_manager();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    test_mission_manager();
}

void loop() {}
#endif
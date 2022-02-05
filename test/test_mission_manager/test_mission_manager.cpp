#include "Monitors/FaultMonitor.hpp"
#include <MissionManager.hpp>
#include <unity.h>
void test_valid_initialization()
{
    MissionManager mission_manager(0);
    TEST_ASSERT_EQUAL(mission_mode_type::boot, sfr::mission::mode);
}

int test_mission_manager()
{
    UNITY_BEGIN();
    RUN_TEST(test_valid_initialization);
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
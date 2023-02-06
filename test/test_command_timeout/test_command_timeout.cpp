#include <unity.h>
#include "MissionManager.hpp"
#include "Arduino.h"

#define FAKE_ARMED_TIME 5000

void test_armed_timeout()
{
    MissionManager mission_manager(0);

    // Lower actual armed phase timeout
    sfr::burnwire::armed_time.set(FAKE_ARMED_TIME);
    TEST_ASSERT_EQUAL(FAKE_ARMED_TIME, sfr::burnwire::armed_time);

    // Set current mission mode to an armed phase mode
    sfr::mission::current_mode = sfr::mission::normalArmed;
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::armed->get_id(), sfr::mission::current_phase->get_id());

    // Check that armed command timed out
    delay(FAKE_ARMED_TIME + 1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::deployment->get_id(), sfr::mission::current_phase->get_id());
    
}

int test_commands()
{
    UNITY_BEGIN();
    RUN_TEST(test_armed_timeout);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_commands();
}
#else
#include <Arduino.h>
void setup()
{
    delay(5000);
    Serial.begin(9600);
    delay(5000);
    test_commands();
}

void loop() {}
#endif
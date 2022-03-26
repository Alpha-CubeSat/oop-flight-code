#include "MissionManager.hpp"
#include "sfr.hpp"
#include <unity.h>

void reset(MissionManager mission_manager, MissionMode *mode)
{
    sfr::mission::current_mode = mode;
    sfr::mission::max_boot_time = constants::time::two_days;
    sfr::battery::voltage_average->set_value(sfr::battery::acceptable_battery+.1);
    sfr::battery::voltage_average->set_valid();
    TEST_ASSERT_EQUAL(sfr::battery::voltage_average->is_valid(), 1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(mode->id(), sfr::mission::current_mode->id());
}

void test_valid_initialization()
{
    MissionManager mission_manager(0);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::boot->id(), sfr::mission::current_mode->id());
}

void test_exit_boot()
{
    MissionManager mission_manager(0);
    reset(mission_manager, sfr::mission::boot);

    // after max boot time, transition to aliveSignal
    sfr::mission::max_boot_time = 500;
    delay(sfr::mission::max_boot_time);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::aliveSignal->id(), sfr::mission::current_mode->id());
}

void test_exit_alive_signal()
{
    MissionManager mission_manager(0);
    reset(mission_manager, sfr::mission::aliveSignal);

    // exit if voltage sensor fails
    sfr::battery::voltage_average->set_invalid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::lowPowerAliveSignal->id(), sfr::mission::current_mode->id());

    reset(mission_manager, sfr::mission::aliveSignal);

    // exit if battery voltage is too low
    sfr::battery::voltage_average->set_value(sfr::battery::min_battery-.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::lowPowerAliveSignal->id(), sfr::mission::current_mode->id());

    reset(mission_manager, sfr::mission::aliveSignal);

    // exit if battery voltage is too low and voltage sensor fails
    sfr::battery::voltage_average->set_invalid();
    sfr::battery::voltage_average->set_value(sfr::battery::min_battery-.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::lowPowerAliveSignal->id(), sfr::mission::current_mode->id());
}


void test_exit_low_power_alive_signal()
{
    MissionManager mission_manager(0);
    reset(mission_manager, sfr::mission::lowPowerAliveSignal);

    // exit if voltage sensor resolves itself
    sfr::battery::voltage_average->set_valid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::aliveSignal->id(), sfr::mission::current_mode->id());

    reset(mission_manager, sfr::mission::lowPowerAliveSignal);

    // exit if battery voltage is high enough
    sfr::battery::voltage_average->set_value(sfr::battery::acceptable_battery+.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::aliveSignal->id(), sfr::mission::current_mode->id());

    reset(mission_manager, sfr::mission::lowPowerAliveSignal);

    // don't exit if voltage sensor resolves itself but battery voltage does not
    sfr::battery::voltage_average->set_valid();
    sfr::battery::voltage_average->set_value(sfr::battery::min_battery-.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::lowPowerAliveSignal->id(), sfr::mission::current_mode->id());

    reset(mission_manager, sfr::mission::lowPowerAliveSignal);

    // don't exit if voltage sensor resolves itself but battery voltage does not
    sfr::battery::voltage_average->set_invalid();
    sfr::battery::voltage_average->set_value(sfr::battery::acceptable_battery+.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::lowPowerAliveSignal->id(), sfr::mission::current_mode->id());
}

int test_mission_manager()
{
    UNITY_BEGIN();
    RUN_TEST(test_valid_initialization);
    RUN_TEST(test_exit_boot);
    RUN_TEST(test_exit_alive_signal);
    RUN_TEST(test_exit_low_power_alive_signal);
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
    delay(5000);
    Serial.begin(9600);
    delay(5000);
    test_mission_manager();
}

void loop() {}
#endif
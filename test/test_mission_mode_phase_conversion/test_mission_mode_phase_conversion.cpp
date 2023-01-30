#include <unity.h>
#include "MissionManager.hpp"
#include "Arduino.h"

void test_conversion()
{
    MissionManager mission_manager(0);

    // Initialization
    TEST_ASSERT_EQUAL(sfr::mission::initialization->get_id(), mission_manager.modeToPhase(sfr::mission::boot)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::initialization->get_id(), mission_manager.modeToPhase(sfr::mission::aliveSignal)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::initialization->get_id(), mission_manager.modeToPhase(sfr::mission::lowPowerAliveSignal)->get_id());

    // Stabilization
    TEST_ASSERT_EQUAL(sfr::mission::stabilization->get_id(), mission_manager.modeToPhase(sfr::mission::detumbleSpin)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::stabilization->get_id(), mission_manager.modeToPhase(sfr::mission::lowPowerDetumbleSpin)->get_id());

    // Standby
    TEST_ASSERT_EQUAL(sfr::mission::standby->get_id(), mission_manager.modeToPhase(sfr::mission::normal)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::standby->get_id(), mission_manager.modeToPhase(sfr::mission::transmit)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::standby->get_id(), mission_manager.modeToPhase(sfr::mission::lowPower)->get_id());

    // Deployment
    TEST_ASSERT_EQUAL(sfr::mission::deployment->get_id(), mission_manager.modeToPhase(sfr::mission::normalDeployment)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::deployment->get_id(), mission_manager.modeToPhase(sfr::mission::transmitDeployment)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::deployment->get_id(), mission_manager.modeToPhase(sfr::mission::lowPowerDeployment)->get_id());

    // Armed
    TEST_ASSERT_EQUAL(sfr::mission::armed->get_id(), mission_manager.modeToPhase(sfr::mission::normalArmed)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::armed->get_id(), mission_manager.modeToPhase(sfr::mission::transmitArmed)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::armed->get_id(), mission_manager.modeToPhase(sfr::mission::lowPowerArmed)->get_id());

    // In Sun
    TEST_ASSERT_EQUAL(sfr::mission::inSun->get_id(), mission_manager.modeToPhase(sfr::mission::normalInSun)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::inSun->get_id(), mission_manager.modeToPhase(sfr::mission::transmitInSun)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::inSun->get_id(), mission_manager.modeToPhase(sfr::mission::lowPowerInSun)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::inSun->get_id(), mission_manager.modeToPhase(sfr::mission::voltageFailureInSun)->get_id());

    // Firing
    TEST_ASSERT_EQUAL(sfr::mission::firing->get_id(), mission_manager.modeToPhase(sfr::mission::bootCamera)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::firing->get_id(), mission_manager.modeToPhase(sfr::mission::mandatoryBurns)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::firing->get_id(), mission_manager.modeToPhase(sfr::mission::regularBurns)->get_id());
    TEST_ASSERT_EQUAL(sfr::mission::firing->get_id(), mission_manager.modeToPhase(sfr::mission::photo)->get_id());
    
}

int test_mission_mode_phase_conversion()
{
    UNITY_BEGIN();
    RUN_TEST(test_conversion);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_mission_mode_phase_conversion();
}
#else
#include <Arduino.h>
void setup()
{
    delay(5000);
    Serial.begin(9600);
    delay(5000);
    test_mission_mode_phase_conversion();
}

void loop() {}
#endif
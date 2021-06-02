#include <unity.h>
#include <MissionManager.hpp>
#include "Monitors/FaultMonitor.hpp"
void test_valid_initialization(){
    MissionManager mission_manager(0);
    TEST_ASSERT_EQUAL(mission_mode_type::standby, sfr::mission::mode);
}

void test_execute(){
    MissionManager mission_manager(0);
    FaultMonitor fault_monitor(0);
    TEST_ASSERT_EQUAL(mission_mode_type::standby, sfr::mission::mode);

    sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_x;  
    mission_manager.execute();
    fault_monitor.execute();
    TEST_ASSERT_EQUAL(mission_mode_type::safe, sfr::mission::mode);

}

int test_mission_manager() {
    UNITY_BEGIN();
    RUN_TEST(test_valid_initialization);
    RUN_TEST(test_execute);
    return UNITY_END();
}


#ifdef DESKTOP
int main() {
    return test_mission_manager();
}
#else
#include <Arduino.h>
void setup() {
    delay(2000);
    Serial.begin(9600);
    test_mission_manager();
}

void loop() {}
#endif
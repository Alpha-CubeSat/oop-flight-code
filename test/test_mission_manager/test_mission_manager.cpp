#ifdef UNIT_TEST
#include <unity.h>
#include <MissionManager.hpp>
void test_valid_initialization(){
    MissionManager mission_manager(0);
    TEST_ASSERT_EQUAL(mission_mode_type::standby, sfr::mission::mode);
}

void test_execute(){
    MissionManager mission_manager(0);
    TEST_ASSERT_EQUAL(mission_mode_type::standby, sfr::mission::mode);

    sfr::fault::is_fault = true;  
    mission_manager.execute();
    TEST_ASSERT_EQUAL(mission_mode_type::safe, sfr::mission::mode);

}

int test_mission_manager() {
    UNITY_BEGIN();
    RUN_TEST(test_valid_initialization);
    RUN_TEST(test_execute);
    return UNITY_END();
}

#include <Arduino.h>
void setup() {
    delay(2000);
    Serial.begin(9600);
    test_mission_manager();
}

void loop() {}
int main() { int test = 1; return test;}
#endif
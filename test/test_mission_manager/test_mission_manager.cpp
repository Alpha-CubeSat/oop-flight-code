#include <unity.h>
#include <MissionManager.hpp>

void test_valid_initialization(){
    MissionManager mission_manager;
    //TEST_ASSERT_EQUAL(mission_mode_type::ascent, sfr::mission::mode);
}

void test_execute(){
    MissionManager mission_manager;
    //TEST_ASSERT_EQUAL(mission_mode_type::ascent, sfr::mission::mode);

    //sfr::gps::altitude = constants::gps::mand_deploy + 1;
    mission_manager.execute();
    //TEST_ASSERT_EQUAL(mission_mode_type::deploy_lightsail, sfr::mission::mode);

    sfr::photoresistor::covered = true;
    mission_manager.execute();
    //TEST_ASSERT_EQUAL(true, sfr::burnwire::on);

    sfr::photoresistor::covered = false;
    mission_manager.execute();
    //TEST_ASSERT_EQUAL(mission_mode_type::taking_photo, sfr::mission::mode);

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
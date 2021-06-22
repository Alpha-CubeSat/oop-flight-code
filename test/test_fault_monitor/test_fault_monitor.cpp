#include <unity.h>
#include "Monitors/FaultMonitor.hpp"
#include "MissionManager.hpp"

void test_stay_in_safe_mode(){
    FaultMonitor fault_monitor(0);
    MissionManager mission_manager(0);

    sfr::temperature::temp_c = constants::temperature::max_temp_c + 1;
    fault_monitor.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(mission_mode_type::safe, sfr::mission::mode);
    TEST_ASSERT_EQUAL(0, sfr::fault::fault_1);
    //temp_c is at position 1, so binary -> decimal is 2
    TEST_ASSERT_EQUAL(2, sfr::fault::fault_2);
    TEST_ASSERT_EQUAL(0, sfr::fault::fault_3);

    for(int i=0; i <100; ++i){
        sfr::temperature::temp_c = 0;
        sfr::fault::fault_1 = 0;
        sfr::fault::fault_2 = 0;
        sfr::fault::fault_3 = 0;
        fault_monitor.execute();
        mission_manager.execute();
        TEST_ASSERT_EQUAL(mission_mode_type::safe, sfr::mission::mode);
    }
}

int test_burnwire() {
    UNITY_BEGIN();
    RUN_TEST(test_stay_in_safe_mode);
    return UNITY_END();
}


#ifdef DESKTOP
int main() {
    return test_burnwire();
}
#else
#include <Arduino.h>
void setup() {
    delay(2000);
    Serial.begin(9600);
    test_burnwire();
}

void loop() {}
#endif
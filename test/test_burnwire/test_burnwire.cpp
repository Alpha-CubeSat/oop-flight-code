#include <unity.h>
#include "Control Tasks/BurnwireControlTask.hpp"
void test_valid_initialization(){
    BurnwireControlTask burnwire_control_task(0);
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_execute(){
    

}

int test_burnwire() {
    UNITY_BEGIN();
    RUN_TEST(test_valid_initialization);
    RUN_TEST(test_execute);
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
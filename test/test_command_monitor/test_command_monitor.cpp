#include <unity.h>
#include <Monitors/CommandMonitor.hpp>

void test_initialize() {
    CommandMonitor command_monitor(0);
    TEST_ASSERT_EQUAL(0, 0);
}

int test_simulator() {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    return UNITY_END();
}

#ifdef DESKTOP
int main() {
    return test_simulator();
}
#else
#include <Arduino.h>
void setup() {
    delay(2000);
    Serial.begin(9600);
    test_simulator();
}

void loop() {}
#endif
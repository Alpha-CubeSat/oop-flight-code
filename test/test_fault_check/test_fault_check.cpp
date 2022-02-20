#include "FaultCheck.hpp"
#include <unity.h>

void test_dummy()
{
    int *bits = FaultCheck::flip_flag(true);
    Serial.printf(bits[0]);
}

int test_fault_check()
{
    UNITY_BEGIN();
    RUN_TEST(test_dummy);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_fault_check();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    test_fault_check();
}

void loop() {}
#endif
#include "sfr.hpp"
#include "unity.h"
#include <Monitors/ButtonMonitor.hpp>

#define RAND_MAX __RAND_MAX = 1023;

void test_valid_readings()
{
    TEST_ASSERT_TRUE(sfr::button::button_pressed->is_valid());
}

void test_invalid_readings() {
    sfr::button::button_pressed->set_invalid();
    TEST_ASSERT_FALSE(sfr::button::button_pressed->is_valid());
}

int test_button()
{
    UNITY_BEGIN();
    RUN_TEST(test_valid_readings);
    RUN_TEST(test_invalid_readings);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_button();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    test_button();
}

void loop() {}
#endif
#include "sfr.hpp"
#include "unity.h"
#include <Monitors/ButtonMonitor.hpp>

#define RAND_MAX __RAND_MAX = 1023;

void test_valid_readings()
{
    // pressed readings in normal mode (valid)
    sfr::button::button_pressed->set_value(1);
    while (sfr::mission::mode_history.size() < constants::sensor::collect) {
        sfr::mission::mode_history.push_front(5);
    }
    ButtonMonitor button_monitor(0);
    TEST_ASSERT_EQUAL(sfr::button::button_pressed->is_valid(), true);
    sfr::button::button_pressed->set_valid();

    // unpressed readings in normal mode (invalid)
    sfr::button::button_pressed->set_value(0);
    while (sfr::mission::mode_history.size() < constants::sensor::collect) {
        sfr::mission::mode_history.push_front(5);
    }
    TEST_ASSERT_EQUAL(sfr::button::button_pressed->is_valid(), false);
}

int test_button()
{
    UNITY_BEGIN();
    RUN_TEST(test_valid_readings);
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
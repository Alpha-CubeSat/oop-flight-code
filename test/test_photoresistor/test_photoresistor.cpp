#include "sfr.hpp"
#include "unity.h"
#include <Monitors/PhotoresistorMonitor.hpp>

#define RAND_MAX __RAND_MAX = 1023;

void test_repeated_values()
{
    // unique values (valid)
    while (sfr::photoresistor::light_val_buffer.size() < constants::sensor::collect) {
        sfr::photoresistor::light_val_buffer.push_front(rand());
    }
    PhotoresistorMonitor photoresistor_monitor(0);
    TEST_ASSERT_EQUAL(photoresistor_monitor.check_repeated_values(sfr::photoresistor::light_val_buffer), false);
    sfr::photoresistor::light_val_buffer.clear();

    // repeated values (invalid)
    while (sfr::photoresistor::light_val_buffer.size() < constants::sensor::collect) {
        sfr::photoresistor::light_val_buffer.push_front(0);
    }
    TEST_ASSERT_EQUAL(photoresistor_monitor.check_repeated_values(sfr::photoresistor::light_val_buffer), true);
    sfr::photoresistor::light_val_buffer.clear();
}

void test_valid_readings()
{
    // dark readings in normal mode (valid)
    while (sfr::photoresistor::light_val_buffer.size() < constants::sensor::collect) {
        sfr::photoresistor::light_val_buffer.push_front(50);
    }
    while (sfr::mission::mode_history.size() < constants::sensor::collect) {
        sfr::mission::mode_history.push_front(5);
    }
    PhotoresistorMonitor photoresistor_monitor(0);
    TEST_ASSERT_EQUAL(photoresistor_monitor.check_invalid_reading(sfr::photoresistor::light_val_buffer), false);
    sfr::photoresistor::light_val_buffer.clear();

    // light readings in normal mode (invalid)
    while (sfr::photoresistor::light_val_buffer.size() < constants::sensor::collect) {
        sfr::photoresistor::light_val_buffer.push_front(1000);
    }
    while (sfr::mission::mode_history.size() < constants::sensor::collect) {
        sfr::mission::mode_history.push_front(5);
    }
    TEST_ASSERT_EQUAL(photoresistor_monitor.check_invalid_reading(sfr::photoresistor::light_val_buffer), true);
}

int test_photoresistor()
{
    UNITY_BEGIN();
    RUN_TEST(test_repeated_values);
    RUN_TEST(test_valid_readings);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_photoresistor();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    test_photoresistor();
}

void loop() {}
#endif
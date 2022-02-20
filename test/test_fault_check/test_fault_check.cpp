#include "FaultCheck.hpp"
#include <unity.h>

void test_flip()
{
    bool flag_true = 1;
    bool flag_false = 0;
    bool fault_true = true;
    bool fault_false = false;
    FaultCheck::flip_flag(flag_true);
    FaultCheck::flip_flag(flag_false);
    FaultCheck::flip_fault(fault_true);
    FaultCheck::flip_fault(fault_false);
    TEST_ASSERT_EQUAL(flag_true, 0);
    TEST_ASSERT_EQUAL(flag_false, 1);
    TEST_ASSERT_EQUAL(fault_true, 0);
    TEST_ASSERT_EQUAL(fault_false, 1);
}

void test_check_is_fault()
{
    sfr::fault::check_gyro_x = true;
    bool gyro_x_check = FaultCheck::check_is_fault(fault_check_type::gyro_x);
    TEST_ASSERT_EQUAL(gyro_x_check, sfr::fault::check_gyro_x);
}
void test_check_is_flag()
{
    bool dummy_flag_check = FaultCheck::check_is_flag();
    TEST_ASSERT_EQUAL(dummy_flag_check, false);
}

int test_fault_check()
{
    UNITY_BEGIN();
    RUN_TEST(test_flip);
    RUN_TEST(test_check_is_fault);
    RUN_TEST(test_check_is_flag);
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
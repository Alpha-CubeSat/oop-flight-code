#include <unity.h>

void test_execute()
{
}

int test_pio_test()
{
    UNITY_BEGIN();
    RUN_TEST(test_execute);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_pio_test();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    test_pio_test();
}

void loop() {}
#endif
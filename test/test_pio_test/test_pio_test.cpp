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
    delay(5000);
    Serial.begin(9600);
    delay(5000);
    Serial.println("Hello");
    test_pio_test();
}

void loop() {}
#endif
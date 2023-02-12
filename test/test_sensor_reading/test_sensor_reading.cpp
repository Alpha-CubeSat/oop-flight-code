#include <unity.h>
#include "SensorReading.hpp"
#include "Arduino.h"

void test_buffer_creation()
{
    SensorReading test_values = new SensorReading(10, -100, 100);

    test_values.set_value( 50 );

    Serial.println(test.values.get_buffer());
    
}

int test_sensor_reading()
{
    UNITY_BEGIN();
    RUN_TEST(test_buffer_creation);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_sensor_reading();
}
#else
#include <Arduino.h>
void setup()
{
    delay(5000);
    Serial.begin(9600);
    delay(5000);
    test_sensor_reading();
}

void loop() {}
#endif
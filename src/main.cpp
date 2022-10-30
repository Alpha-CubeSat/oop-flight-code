#include "Control Tasks/PinControlTask.hpp"
#include "MainControlLoop.hpp"

MainControlLoop mcl;
PinControlTask pin_control_task;

#ifndef UNIT_TEST

void setup()
{
    // delay for 2 hours
    // delay(7200000);
    Serial.begin(9600);
    delay(5000);
    pin_control_task.execute();

    // uint32_t opcodes[(int)sfr::rockblock::max_commands_count];
    // uint32_t args_1[(int)sfr::rockblock::max_commands_count];
    // uint32_t args_2[(int)sfr::rockblock::max_commands_count];

    // opcodes[0] = (uint32_t)(6401);
    // args_1[0] = (uint32_t)(286331153);
    // args_2[0] = (uint32_t)(0);

    // Serial.println(opcodes[0], HEX);
    // Serial.println(args_1[0], HEX);
    // Serial.println(args_2[0], HEX);

    // uint32_t calculated_checksum = 0;
    // for (int j = 0; j < 1; j++) {
    //     // Longitudinal redundancy check, kinda
    //     calculated_checksum = calculated_checksum ^ (opcodes[j] ^ args_1[j] ^ args_2[j]);
    // }
    // Serial.print(calculated_checksum, HEX);
}

void loop()
{
    mcl.execute();
}
#endif
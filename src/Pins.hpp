#ifndef PINS_HPP_
#define PINS_HPP_

class Pins
{
public:
    static void setPinState(int pinName, int pinState);
    static int getPinState(int pinName);
};

#endif
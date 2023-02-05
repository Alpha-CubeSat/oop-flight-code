#ifndef PINS_HPP_
#define PINS_HPP_

#include "sfr.hpp"

class Pins
{
public:
    static void setPinState(int pinName, int pinState);
    static int getPinState(int pinName);
    static void setInitialPinStates();
};

#endif
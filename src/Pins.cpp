#include "Pins.hpp"

void Pins::setPinState(int pinName, int pinState)
{
  sfr::pins::pinMap[pinName] = pinState;
  digitalWrite(pinName, LOW);
}

int Pins::getPinState(int pinName)
{
  return sfr::pins::pinMap[pinName];
}
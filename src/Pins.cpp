#include "Pins.hpp"

void Pins::setPinState(int pinName, int pinState)
{
    sfr::pins::pinMap[pinName] = pinState;
    digitalWrite(pinName, pinState);
}

int Pins::getPinState(int pinName)
{
    return sfr::pins::pinMap[pinName];
}

void Pins::setInitialPinStates()
{
    pinMode(constants::acs::xPWMpin, OUTPUT);
    pinMode(constants::acs::yPWMpin, OUTPUT);
    pinMode(constants::acs::zPWMpin, OUTPUT);
    pinMode(constants::acs::yout1, OUTPUT);
    pinMode(constants::acs::yout2, OUTPUT);
    pinMode(constants::acs::xout1, OUTPUT);
    pinMode(constants::acs::xout2, OUTPUT);
    pinMode(constants::acs::zout1, OUTPUT);
    pinMode(constants::acs::zout2, OUTPUT);
    pinMode(constants::camera::power_on_pin, OUTPUT);
    pinMode(constants::camera::rx, OUTPUT);
    pinMode(constants::camera::tx, OUTPUT);
    pinMode(constants::button::button_pin, INPUT);
    pinMode(constants::battery::voltage_value_pin, INPUT);
    pinMode(constants::acs::STBXYpin, OUTPUT);
    pinMode(constants::acs::STBZpin, OUTPUT);
    pinMode(constants::photoresistor::pin, INPUT);
    pinMode(constants::temperature::pin, INPUT);
    pinMode(constants::current::pin, INPUT);
    pinMode(constants::burnwire::first_pin, OUTPUT);
    pinMode(constants::burnwire::second_pin, OUTPUT);
    pinMode(constants::rockblock::sleep_pin, OUTPUT);

    Pins::setPinState(constants::acs::xPWMpin, LOW);
    Pins::setPinState(constants::acs::yPWMpin, LOW);
    Pins::setPinState(constants::acs::zPWMpin, LOW);
    Pins::setPinState(constants::acs::yout1, LOW);
    Pins::setPinState(constants::acs::yout2, LOW);
    Pins::setPinState(constants::acs::xout1, LOW);
    Pins::setPinState(constants::acs::xout2, LOW);
    Pins::setPinState(constants::acs::zout1, LOW);
    Pins::setPinState(constants::acs::zout2, LOW);
    Pins::setPinState(constants::camera::power_on_pin, LOW);
    Pins::setPinState(constants::camera::rx, LOW);
    Pins::setPinState(constants::camera::tx, LOW);
    Pins::setPinState(constants::acs::STBXYpin, LOW);
    Pins::setPinState(constants::acs::STBZpin, LOW);
    Pins::setPinState(constants::burnwire::first_pin, LOW);
    Pins::setPinState(constants::burnwire::second_pin, LOW);
    Pins::setPinState(constants::rockblock::sleep_pin, HIGH);
}
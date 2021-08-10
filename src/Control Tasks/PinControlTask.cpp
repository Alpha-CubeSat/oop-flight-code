#include "PinControlTask.hpp"

PinControlTask::PinControlTask(){

    //serial for rockblock?
    //i2c for imu?
    //serial for camera?

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
    pinMode(constants::battery::allow_measurement_pin, OUTPUT);
    pinMode(constants::burnwire::second_pin, OUTPUT);
    pinMode(constants::rockblock::sleep_pin, OUTPUT);
}
        

void PinControlTask::execute(){   
    digitalWrite(constants::acs::xPWMpin, LOW);
    digitalWrite(constants::acs::yPWMpin, LOW);
    digitalWrite(constants::acs::zPWMpin, LOW);
    digitalWrite(constants::acs::yout1, LOW);
    digitalWrite(constants::acs::yout2, LOW);
    digitalWrite(constants::acs::xout1, LOW);
    digitalWrite(constants::acs::xout2, LOW);
    digitalWrite(constants::acs::zout1, LOW);
    digitalWrite(constants::acs::zout2, LOW);
    digitalWrite(constants::camera::power_on_pin, LOW);
    digitalWrite(constants::acs::STBXYpin, LOW);
    digitalWrite(constants::acs::STBZpin, LOW);
    digitalWrite(constants::burnwire::first_pin, LOW);
    digitalWrite(constants::battery::allow_measurement_pin, HIGH);
    digitalWrite(constants::burnwire::second_pin, LOW);
    digitalWrite(constants::rockblock::sleep_pin, HIGH);
}
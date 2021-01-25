#include "MainControlLoop.hpp"
#include <Arduino.h>

void setup(){
    MainControlLoop mcl = MainControlLoop();

    //delay for 2 hours
    //delay(7200000);
    while(true){
        int old = millis();
        mcl.execute();
        int time = millis() - old;
        Serial.println(time);
    }
}

void loop(){}



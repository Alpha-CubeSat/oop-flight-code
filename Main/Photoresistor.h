#ifndef Photoresistor_H
#define Photoresistor_H

#include <Arduino.h>

class Photoresistor{
    public:
    Photoresistor(int p);
    bool doorOpen();
    int getPin(){
        return pin;
    }

    private:
    int pin;  
};

#endif
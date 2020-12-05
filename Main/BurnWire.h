#ifndef BurnWire_H
#define BurnWire_H

#include <Arduino.h> 
#include <usb_serial.h>

class BurnWire{
    public:
    BurnWire(int pin, usb_serial_class *swSerial);
    void on();
    void off();
    usb_serial_class getSoftwareSerial(){
        return *softwareSerial;
    }
    
    private:
    int pin; 
    usb_serial_class *softwareSerial; 
};

#endif
#ifndef CAMERA_H
#define CAMERA_H

#include <HardwareSerial.h>
#include <SD.h>
#include <usb_serial.h>
#include <Adafruit_VC0706.h>

class Camera{
    public:
    Camera(HardwareSerial *hwSerial, usb_serial_class *swSerial);
    void setup();
    void takePhoto();
    usb_serial_class getSoftwareSerial(){
        return *softwareSerial;
    }
    
    private:
    usb_serial_class *softwareSerial;
    Adafruit_VC0706 adaCam;    
};

#endif
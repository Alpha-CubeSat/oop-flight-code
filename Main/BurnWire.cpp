#include "BurnWire.h"

BurnWire::BurnWire(int p, usb_serial_class *swSer){
  pin= p;
  softwareSerial = swSer;
}

void BurnWire::on(){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  getSoftwareSerial().println("Turned on burn wire");
}

void BurnWire::off(){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  getSoftwareSerial().println("Turned off wire");
}
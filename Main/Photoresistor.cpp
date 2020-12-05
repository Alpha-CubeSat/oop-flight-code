#include "Photoresistor.h"

Photoresistor::Photoresistor(int p){
  this->pin= p;
}

bool Photoresistor::doorOpen(){
  int lightVal = analogRead(getPin());
  if(lightVal > 400){
    return true;
  }
  else{
    return false;
  }
}
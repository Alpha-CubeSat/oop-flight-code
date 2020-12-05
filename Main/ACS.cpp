#include "ACS.h"

ACS::ACS() : detuble(true), imu(IMU()) {
}

void ACS::setup(){
  //H-bridge standby pins
  pinMode(Constants::STBYPIN, OUTPUT);  
  pinMode(Constants::STBZPIN, OUTPUT);

  //Pins for X-Torquer
  pinMode(Constants::XPWMPIN, OUTPUT);
  pinMode(Constants::XOUT1, OUTPUT);
  pinMode(Constants::XOUT2, OUTPUT);

  //Pins for Y-Torquer
  pinMode(Constants::YPWMPIN, OUTPUT);
  pinMode(Constants::YOUT1, OUTPUT);
  pinMode(Constants::YOUT2, OUTPUT);

  //Pins for Z-Torquer
  pinMode(Constants::ZOUT1, OUTPUT);
  pinMode(Constants::ZOUT2, OUTPUT);
  pinMode(Constants::ZPWMPIN, OUTPUT);

  rtObj.initialize();
  imu.setup();
}

void ACS::ACSwrite(float current,  int out1,  int out2, int PWMpin) {
  if (current == 0.0) {
    digitalWrite(out1, 0);
    digitalWrite(out2, 0);
  }
  if (current > 0) {
    digitalWrite(out1, 0);
    digitalWrite(out2, 1);
    analogWrite(PWMpin, current);
  }
  if (current < 0) {
    digitalWrite(out1, 1);
    digitalWrite(out2, 0);
    analogWrite(PWMpin, current);
  } 
}

void ACS::setCurrent(){
  rtObj.rtU.angularvelocity[0] = imu.getGyroX();
  rtObj.rtU.angularvelocity[1] = imu.getGyroY();
  rtObj.rtU.angularvelocity[2] = imu.getGyroZ();
  rtObj.rtU.Bfield_body[0] = imu.getMagX();
  rtObj.rtU.Bfield_body[1] = imu.getMagY();
  rtObj.rtU.Bfield_body[2] = imu.getMagZ();
  rtObj.step();

  if(detuble){
      current1 = rtObj.rtY.detuble[0];
      current2 = rtObj.rtY.detuble[1];
      current3 = rtObj.rtY.detuble[2];
  }
  if(!detuble){
      current1 = rtObj.rtY.point[0];
      current2 = rtObj.rtY.point[1];
      current3 = rtObj.rtY.point[2];
  }
}

void ACS::run(){
  ACSwrite(current1, Constants::XOUT1, Constants::XOUT2, Constants::XPWMPIN);
  ACSwrite(current2, Constants::YOUT1, Constants::YOUT2, Constants::YPWMPIN); 
  ACSwrite(current3, Constants::ZOUT1, Constants::ZOUT2, Constants::ZPWMPIN);
}

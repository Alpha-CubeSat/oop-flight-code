#include "ACSControlTask.hpp"

ACSControlTask::ACSControlTask(){
    //H-bridge standby pins
    pinMode(constants::acs::STBYpin, OUTPUT);  
    pinMode(constants::acs::STBZpin, OUTPUT);

    //Pins for X-Torquer
    pinMode(constants::acs::xPWMpin, OUTPUT);
    pinMode(constants::acs::xout1, OUTPUT);
    pinMode(constants::acs::xout2, OUTPUT);

    //Pins for Y-Torquer
    pinMode(constants::acs::yPWMpin, OUTPUT);
    pinMode(constants::acs::yout1, OUTPUT);
    pinMode(constants::acs::yout2, OUTPUT);

    //Pins for Z-Torquer
    pinMode(constants::acs::zPWMpin, OUTPUT);
    pinMode(constants::acs::zout1, OUTPUT);
    pinMode(constants::acs::zout2, OUTPUT);

    rtObj.initialize();
}

void ACSControlTask::execute(){ 
    float current1;
    float current2;
    float current3;

    rtObj.rtU.angularvelocity[0] = sfr::imu::gyro_x * 3.14159 / 180.0; 
    rtObj.rtU.angularvelocity[1] = sfr::imu::gyro_y * 3.14159 / 180.0;
    rtObj.rtU.angularvelocity[2] = sfr::imu::gyro_z * 3.14159 / 180.0;
    rtObj.rtU.Bfield_body[0] = sfr::imu::mag_x;
    rtObj.rtU.Bfield_body[1] = sfr::imu::mag_y;
    rtObj.rtU.Bfield_body[2] = sfr::imu::mag_z;
    rtObj.step();

    if(sfr::acs::detuble){
        current1 = rtObj.rtY.detuble[0];
        current2 = rtObj.rtY.detuble[1];
        current3 = rtObj.rtY.detuble[2];
    }
    else{
        current1 = rtObj.rtY.point[0];
        current2 = rtObj.rtY.point[1];
        current3 = rtObj.rtY.point[2];
    }

    ACSWrite(constants::acs::xtorqorder, current1, constants::acs::xout1, constants::acs::xout2, constants::acs::xPWMpin);
    ACSWrite(constants::acs::ytorqorder, current2, constants::acs::yout1, constants::acs::yout2, constants::acs::yPWMpin);
    ACSWrite(constants::acs::ztorqorder, current3, constants::acs::zout1, constants::acs::zout2, constants::acs::zPWMpin);
}

void ACSControlTask::ACSWrite(int torqorder,  float current,  int out1,  int out2, int PWMpin){
    if (current == 0.0) {
        digitalWrite(out1, LOW);
        digitalWrite(out2, LOW);
    }
    if (torqorder == 0) { 
        if (current > 0) {
            digitalWrite(out1, HIGH);
            digitalWrite(out2, LOW);
            analogWrite(PWMpin, current);
        }
        if (current < 0) {
            digitalWrite(out1, LOW);
            digitalWrite(out2, HIGH);
            analogWrite(PWMpin, current);
        }
    }
    if (torqorder == 1) {
        if (current > 0) {
            digitalWrite(out1, LOW);
            digitalWrite(out2, HIGH);
            analogWrite(PWMpin, current);
        }
        if (current < 0) {
            digitalWrite(out1, HIGH);
            digitalWrite(out2, LOW);
            analogWrite(PWMpin, current);
        }
    }
}
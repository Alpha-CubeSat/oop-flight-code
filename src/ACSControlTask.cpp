#include "ACSControlTask.hpp"

ACSControlTask::ACSControlTask(){
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
}

void ACSControlTask::execute(){    
    
}
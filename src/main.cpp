#include "MainControlLoop.hpp"
#include "sfr.hpp"

MainControlLoop mcl;

#ifndef UNIT_TEST
void setup(){
    //delay for 2 hours
    //delay(7200000);
    delay(2000);
    
}

void loop(){
    mcl.execute();
}
#endif



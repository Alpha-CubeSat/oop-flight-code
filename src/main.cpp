#include "MainControlLoop.hpp"

MainControlLoop mcl;

#ifndef GIT
void setup(){
    //delay for 2 hours
    //delay(7200000);
    delay(2000);
    
}

void loop(){
    mcl.execute();
}
#else
#ifndef UNIT_TEST
int main(){
    mcl.execute();
}
#endif
#endif



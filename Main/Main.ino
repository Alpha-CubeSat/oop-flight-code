
#include "ACS.h"
#include "IMU.h"

//TODO: run every 10ms, set pin values correctly

ACS acs = ACS();

void setup(){
    acs.setup();
}

void loop(){
    acs.run();
}

#include "CurrentMonitor.hpp"

CurrentMonitor::CurrentMonitor(){
    pinMode(constants::current::pin, OUTPUT);
}

void CurrentMonitor::execute(){    
    sfr::current::solar_current = map(analogRead(constants::current::pin), 0, 1024, 0, 255);
}
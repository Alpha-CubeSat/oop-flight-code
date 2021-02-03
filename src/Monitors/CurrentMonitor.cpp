#include "CurrentMonitor.hpp"

CurrentMonitor::CurrentMonitor(){
    pinMode(constants::current::pin, OUTPUT);
}

void CurrentMonitor::execute(){    
    sfr::current::solar_current = analogRead(constants::current::pin);
}
#include "CurrentMonitor.hpp"

CurrentMonitor::CurrentMonitor(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::current::pin, OUTPUT);
}

void CurrentMonitor::execute(){    
    sfr::current::solar_current = analogRead(constants::current::pin);
}
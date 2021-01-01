#include "CurrentMonitor.hpp"

CurrentMonitor::CurrentMonitor(){
    pinMode(constants::current::pin, OUTPUT);
}

void CurrentMonitor::execute(){    
    
}
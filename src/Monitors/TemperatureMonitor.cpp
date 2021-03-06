#include "TemperatureMonitor.hpp"

TemperatureMonitor::TemperatureMonitor(){
    pinMode(constants::temperature::pin, INPUT);
}

void TemperatureMonitor::execute(){    
    sfr::temperature::temp_c = (((analogRead(constants::temperature::pin) * 3.3)/ 1023) - .5) * 100; 
}
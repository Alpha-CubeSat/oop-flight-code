#include "TemperatureMonitor.hpp"

TemperatureMonitor::TemperatureMonitor(){
    pinMode(constants::burnwire::pin, INPUT);
}

void TemperatureMonitor::execute(){    
    sfr::temperature::raw_temp = analogRead(constants::temperature::pin);
}
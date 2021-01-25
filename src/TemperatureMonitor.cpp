#include "TemperatureMonitor.hpp"

TemperatureMonitor::TemperatureMonitor(){
    pinMode(constants::burnwire::pin, INPUT);
}

void TemperatureMonitor::execute(){    
    int reading = analogRead(constants::temperature::pin);
    float voltage = reading * 3.3;
    voltage /= 1024.0;
    sfr::temperature::temp_c = (voltage - 0.5) * 100;
}
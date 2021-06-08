#include "TemperatureMonitor.hpp"

TemperatureMonitor::TemperatureMonitor(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::temperature::pin, INPUT);
}

void TemperatureMonitor::execute(){
    float total = 0.0;
    for(size_t i = 0; i < constants::sensor::collect; i++) {
        total += (((analogRead(constants::temperature::pin) * 3.3)/ 1023) - .5) * 100;
    }
    sfr::temperature::temp_c = total / constants::sensor::collect;
}
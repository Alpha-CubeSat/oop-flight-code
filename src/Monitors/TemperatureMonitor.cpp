#include "TemperatureMonitor.hpp"

TemperatureMonitor::TemperatureMonitor(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::temperature::pin, INPUT);
}

void TemperatureMonitor::execute(){
    float total = 0.0;
    float val;
    for(size_t i = 0; i < constants::sensor::collect; i++) {
        val = (((analogRead(constants::temperature::pin) * 3.3)/ 1023) - .5) * 100;
        total += val;
    }
    sfr::temperature::temp_c = val;
    sfr::temperature::temp_c_average = total / constants::sensor::collect;
}
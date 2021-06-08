#include "CurrentMonitor.hpp"

CurrentMonitor::CurrentMonitor(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::current::pin, OUTPUT);
}

void CurrentMonitor::execute(){
    float total = 0.0;
    int val;
    for(size_t i = 0; i < constants::sensor::collect; i++) {
        val = analogRead(constants::current::pin);
        total += val;
    }
    sfr::current::solar_current = val;
    sfr::current::solar_current_average = total / constants::sensor::collect;
}
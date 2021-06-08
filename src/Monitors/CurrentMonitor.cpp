#include "CurrentMonitor.hpp"

CurrentMonitor::CurrentMonitor(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::current::pin, OUTPUT);
}

void CurrentMonitor::execute(){
    float total = 0.0;
    for(size_t i = 0; i < constants::sensor::collect; i++) {
        total += analogRead(constants::current::pin);
    }
    sfr::current::solar_current = total / constants::sensor::collect;
}
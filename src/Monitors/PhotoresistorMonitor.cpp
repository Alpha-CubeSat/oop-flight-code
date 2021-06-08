#include "PhotoresistorMonitor.hpp"

PhotoresistorMonitor::PhotoresistorMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void PhotoresistorMonitor::execute(){
    int total = 0;
    for(size_t i = 0; i < constants::sensor::collect; i++) {
        total += analogRead(constants::photoresistor::pin);
    }
    int avg = total / constants::sensor::collect;
    if(avg > constants::photoresistor::light_val){
        sfr::photoresistor::covered = false;
    }
    else{
        sfr::photoresistor::covered = true;
    }
}
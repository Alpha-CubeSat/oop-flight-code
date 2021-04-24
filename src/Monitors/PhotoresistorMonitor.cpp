#include "PhotoresistorMonitor.hpp"

PhotoresistorMonitor::PhotoresistorMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void PhotoresistorMonitor::execute(){
    if(analogRead(constants::photoresistor::pin) > constants::photoresistor::light_val){
        sfr::photoresistor::covered = false;
    }
    else{
        sfr::photoresistor::covered = true;
    }
}
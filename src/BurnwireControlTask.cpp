#include "BurnwireControlTask.hpp"

BurnwireControlTask::BurnwireControlTask(){
    pinMode(constants::burnwire::pin, OUTPUT);
}

void BurnwireControlTask::execute(){    
    if(sfr::burnwire::on){
        digitalWrite(constants::burnwire::pin, HIGH);
    }
    else{
        digitalWrite(constants::burnwire::pin, LOW);
    }
}
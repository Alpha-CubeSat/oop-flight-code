#include "ButtonMonitor.hpp"

ButtonMonitor::ButtonMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void ButtonMonitor::execute(){
    int val = analogRead(constants::button::pin);
    if(val > constants::button::pressed_val){
        sfr::button::pressed = false;
    }
    else{
        sfr::button::pressed = true;
    }
}

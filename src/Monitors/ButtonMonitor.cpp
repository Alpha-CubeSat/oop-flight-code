#include "ButtonMonitor.hpp"

ButtonMonitor::ButtonMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void ButtonMonitor::execute(){
    float total = 0.0;
    for(size_t i = 0; i < constants::sensor::collect; i++) {
        total += digitalRead(constants::button::button_pin);
    }
    float probability = total / constants::sensor::collect;
    sfr::button::pressed = probability > 0.5; 
}

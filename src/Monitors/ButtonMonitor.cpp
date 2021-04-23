#include "ButtonMonitor.hpp"

ButtonMonitor::ButtonMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void ButtonMonitor::execute(){
    sfr::button::pressed = digitalRead(constants::button::button_pin); 
}

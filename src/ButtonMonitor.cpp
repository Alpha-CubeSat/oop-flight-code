#include "ButtonMonitor.hpp"

ButtonMonitor::ButtonMonitor(){}

void ButtonMonitor::execute(){
    sfr::button::pressed = digitalRead(constants::button::button_pin); 
}

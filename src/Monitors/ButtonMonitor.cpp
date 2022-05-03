#include "ButtonMonitor.hpp"

ButtonMonitor::ButtonMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void ButtonMonitor::execute()
{
    sfr::button::pressed = digitalRead(constants::button::button_pin);
#ifdef VERBOSE
    Serial.print("Button: ");
    Serial.print(sfr::button::pressed);
    Serial.println("(1 yes, 0 no)");
#endif
}
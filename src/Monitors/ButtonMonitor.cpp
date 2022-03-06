#include "ButtonMonitor.hpp"

ButtonMonitor::ButtonMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void ButtonMonitor::execute()
{
    sfr::button::pressed = digitalRead(constants::button::button_pin);

    if (!sfr::button::pressed) {
        sfr::camera::take_photo = true;
        delay(700);
        Serial.println("Button released, taking photo!");
    }

#ifdef VERBOSE
    Serial.print("Button: ");
    Serial.print(sfr::button::pressed);
    Serial.println("(1 yes, 0 no)");
#endif
}
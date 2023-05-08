#include "ButtonMonitor.hpp"

ButtonMonitor::ButtonMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void ButtonMonitor::execute()
{
    float val = digitalRead(constants::button::button_pin);
    sfr::button::button_pressed->set_value(val);

    // button is recognized as not pressed
    if (val == 0) {

        // it is possible for the button to be unpressed
        if (sfr::mission::possible_uncovered) {
            sfr::button::pressed = false;
        } else {
            sfr::button::pressed = true;
            sfr::button::button_pressed->set_invalid();
        }
    } else {
        sfr::button::pressed = true;
    }

#ifdef VERBOSE
    Serial.print("Button: ");
    Serial.print(sfr::button::pressed);
    Serial.println("(1 yes, 0 no)");
    Serial.print("Button valid: ");
    if (sfr::button::button_pressed->is_valid()) {
        Serial.println("true");
    } else {
        Serial.println("false");
    }
#endif
}
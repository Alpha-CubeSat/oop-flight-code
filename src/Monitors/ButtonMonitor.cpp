#include "ButtonMonitor.hpp"

ButtonMonitor::ButtonMonitor()
{
}

void ButtonMonitor::execute()
{
    uint8_t val = digitalRead(constants::button::button_pin);
    sfr::button::button_pressed->set_value(val);
    Serial.print("Button read: ");
    Serial.println(val);
    // button is recognized as not pressed
    if (val == 0) {
        sfr::button::pressed = false;
        // if it is not possible for the button to be unpressed
        if (!sfr::mission::possible_uncovered) {
            sfr::button::button_pressed->set_invalid();
            fault_groups::hardware_faults::button->force();
        }
    } else {
        sfr::button::pressed = true;
    }
}
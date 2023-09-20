#include "ButtonMonitor.hpp"

ButtonMonitor::ButtonMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void ButtonMonitor::execute()
{
    val = digitalRead(constants::button::button_pin);
    sfr::button::button_pressed->set_value(val);

    // button is recognized as not pressed
    if (val == 0) {
        // it is possible for the button to be unpressed
        if (sfr::mission::possible_uncovered) {
            sfr::button::pressed = false;
        } else {
            sfr::button::button_pressed->set_invalid();
            fault_groups::hardware_faults::button->force();
        }
    } else {
        sfr::button::pressed = true;
    }
}
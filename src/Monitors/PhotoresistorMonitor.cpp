#include "PhotoresistorMonitor.hpp"

PhotoresistorMonitor::PhotoresistorMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void PhotoresistorMonitor::execute()
{
    float val = analogRead(constants::photoresistor::pin);
    boolean possible_uncovered = false;

    light_val_average->set_value(val);

    // photoresistor is recognized as uncovered
    if (light_val_average->get_value(&val) && (val > constants::photoresistor::light_val)) {

        for (int mission_mode : sfr::mission::mode_history) {
            // checks if mission mode has never reached burn
            if (mission_mode >= sfr::mission::mandatoryBurns->get_id()) {
                possible_uncovered = true;
            }
        }

        // it is possible for the photoresistor to be uncovered
        if (possible_uncovered) {
            sfr::photoresistor::covered = false;
        } else {
            sfr::photoresistor::covered = true;
            light_val_average->set_invalid();
        }

    } else {
        sfr::photoresistor::covered = true;
    }

#ifdef VERBOSE
    Serial.print("Photoresistor: ");
    Serial.print(sfr::photoresistor::val);
    Serial.println(" (0-1023 scale)");
#endif
}
#include "PhotoresistorMonitor.hpp"

PhotoresistorMonitor::PhotoresistorMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void PhotoresistorMonitor::execute()
{
    if (!initialized) {
        sfr::photoresistor::light_val_average_standby->set_valid();
        sfr::photoresistor::light_val_average_deployment->set_valid();
        initialized = true;
    }

    float val = analogRead(constants::photoresistor::pin);
    bool possible_uncovered = false;

    sfr::photoresistor::light_val_average_standby->set_value(val);
    sfr::photoresistor::light_val_average_deployment->set_value(val);

    // photoresistor is recognized as uncovered
    if ((sfr::photoresistor::light_val_average_standby->get_value(&val) || sfr::photoresistor::light_val_average_deployment->get_value(&val)) && val > constants::photoresistor::light_val) {
        for (int mission_mode : sfr::mission::mode_history) {
            // checks if mission mode has never reached burn
            if (mission_mode >= sfr::mission::mandatoryBurns->get_id()) {
                possible_uncovered = true;
            }
        }

        // is it possible for the photoresistor to be uncovered
        if (possible_uncovered) {
            // photoresistor is recognized as uncovered (via singleton buffer)
            if (sfr::photoresistor::light_val_average_deployment->get_value(&val) && val > constants::photoresistor::light_val) {
                sfr::photoresistor::covered = false;
            }
        } else {
            // photoresistor is recognized as uncovered (via 4 second buffer)
            if (sfr::photoresistor::light_val_average_standby->get_value(&val) && val > constants::photoresistor::light_val) {
                sfr::photoresistor::covered = true;
                sfr::photoresistor::light_val_average_standby->set_invalid();
            }
        }

    } else {
        sfr::photoresistor::covered = true;
    }
}
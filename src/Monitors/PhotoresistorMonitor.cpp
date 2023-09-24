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
    bool avg_buffer_valid = sfr::photoresistor::light_val_average_standby->get_value(&val);
    bool deploy_buffer_valid = sfr::photoresistor::light_val_average_deployment->get_value(&val) && val > constants::photoresistor::light_val;

    // if sun detected, set photoresistor to uncovered (regardless of validity)
    if (val > constants::photoresistor::light_val) {
        sfr::photoresistor::covered = false;
        // if either buffer is valid, check whether to invalidate
        if (avg_buffer_valid || deploy_buffer_valid) {
            for (int mission_mode : sfr::mission::mode_history) {
                // checks if mission mode has never reached burn
                if (mission_mode >= sfr::mission::mandatoryBurns->get_id()) {
                    possible_uncovered = true;
                }
            }

            // set buffer to invalid if never reached burn
            if (!possible_uncovered) {
                if (sfr::photoresistor::light_val_average_standby->get_value(&val)) {
                    sfr::photoresistor::light_val_average_standby->set_invalid();
                } else {
                    sfr::photoresistor::light_val_average_deployment->set_invalid();
                }
                fault_groups::hardware_faults::light_val->force();
            }
        }
        // if dark, set photoresistor to covered
    } else {
        sfr::photoresistor::covered = true;
    }
}
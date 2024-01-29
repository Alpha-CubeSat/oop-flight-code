#include "PhotoresistorMonitor.hpp"

PhotoresistorMonitor::PhotoresistorMonitor()
{
}

void PhotoresistorMonitor::execute()
{

    val = analogRead(constants::photoresistor::pin);
    sfr::photoresistor::light_val_average_standby->set_value(val);
    sfr::photoresistor::light_val_average_deployment->set_value(val);
    avg_buffer_valid = sfr::photoresistor::light_val_average_standby->get_value(&val);
    deploy_buffer_valid = sfr::photoresistor::light_val_average_deployment->get_value(&val) && val > constants::photoresistor::light_val;

    // if sun detected, set photoresistor to uncovered (regardless of validity)
    if (val > constants::photoresistor::light_val) {
        sfr::photoresistor::covered = false;
        // if either buffer is valid, check whether to invalidate
        if (avg_buffer_valid || deploy_buffer_valid) {
            // set buffer to invalid if never reached burn
            if (!sfr::mission::possible_uncovered) {
                sfr::photoresistor::light_val_average_standby->set_invalid();
                sfr::photoresistor::light_val_average_deployment->set_invalid();
                fault_groups::hardware_faults::light_val->force();
            }
        }
        // if dark, set photoresistor to covered
    } else {
        sfr::photoresistor::covered = true;
    }
}
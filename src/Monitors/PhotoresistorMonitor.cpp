#include "PhotoresistorMonitor.hpp"

PhotoresistorMonitor::PhotoresistorMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void PhotoresistorMonitor::execute()
{
    capture_photoresistor_value();
#ifdef VERBOSE
    Serial.print("Photoresistor: ");
    Serial.print(sfr::photoresistor::val);
    Serial.println(" (0-1023 scale)");
#endif
}

bool PhotoresistorMonitor::check_repeated_values(std::deque<int> buffer)
{
    if (buffer.empty() || buffer.size() == 1) {
        return false;
    }

    int first_val = buffer[0];
    for (int val : buffer) {
        if (first_val != val) {
            return false;
        }
    }
    return true;
}

bool PhotoresistorMonitor::check_invalid_reading(std::deque<int> buffer)
{
    for (int val : buffer) {
        if (val > constants::photoresistor::light_val) {
            for (int mission_mode : sfr::mission::mode_history) {
                // checks if mission mode has never reached burn
                if (mission_mode < 19) {
                    return true;
                }
            }
        }
    }
    return false;
}

void PhotoresistorMonitor::capture_photoresistor_value()
{
    sfr::photoresistor::val = analogRead(constants::photoresistor::pin);
    if (sfr::photoresistor::val > constants::photoresistor::light_val) {
        sfr::photoresistor::covered = false;
    } else {
        sfr::photoresistor::covered = true;
    }
    sfr::photoresistor::light_val_buffer.push_front(sfr::photoresistor::val);
    if (sfr::photoresistor::light_val_buffer.size() > constants::sensor::collect) {
        sfr::photoresistor::light_val_buffer.pop_back();
    }
    if (check_repeated_values(sfr::photoresistor::light_val_buffer) || check_invalid_reading(sfr::photoresistor::light_val_buffer)) {
        sfr::photoresistor::light_val_average->set_invalid();
    } else {
        sfr::photoresistor::light_val_average->set_valid();
        int light_val_sum = std::accumulate(sfr::photoresistor::light_val_buffer.begin(), sfr::photoresistor::light_val_buffer.end(), 0);
        sfr::photoresistor::light_val_average->set_value(light_val_sum / sfr::photoresistor::light_val_buffer.size());
    }
}
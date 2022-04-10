#include "PhotoresistorMonitor.hpp"

PhotoresistorMonitor::PhotoresistorMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void PhotoresistorMonitor::execute()
{
    sfr::photoresistor::val = analogRead(constants::photoresistor::pin);
#ifdef VERBOSE
    Serial.print("Photoresistor: ");
    Serial.print(sfr::photoresistor::val);
    Serial.println(" (0-1023 scale)");
#endif
    if (sfr::photoresistor::val > constants::photoresistor::light_val) {
        sfr::photoresistor::covered = false;
    } else {
        sfr::photoresistor::covered = true;
    }
}
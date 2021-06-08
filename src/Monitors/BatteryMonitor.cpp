#include "BatteryMonitor.hpp"

BatteryMonitor::BatteryMonitor(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::battery::voltage_value_pin, OUTPUT);
}

void BatteryMonitor::execute(){
    float total = 0.0;
    int val;
    for(size_t i = 0; i < constants::sensor::collect; i++) {
        val = analogRead(constants::battery::voltage_value_pin);
        total += val;
    }
    sfr::battery::voltage = val;
    sfr::battery::voltage_average = total / constants::sensor::collect;
}

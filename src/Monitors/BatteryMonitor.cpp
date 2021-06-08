#include "BatteryMonitor.hpp"

BatteryMonitor::BatteryMonitor(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::battery::voltage_value_pin, OUTPUT);
}

void BatteryMonitor::execute(){
    float total = 0.0;
    for(size_t i = 0; i < constants::sensor::collect; i++) {
        total += analogRead(constants::battery::voltage_value_pin);
    }
    sfr::battery::voltage = total / constants::sensor::collect;
}

#include "BatteryMonitor.hpp"

BatteryMonitor::BatteryMonitor(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::battery::voltage_value_pin, OUTPUT);
}

void BatteryMonitor::execute(){
    sfr::battery::voltage = analogRead(constants::battery::voltage_value_pin);
}

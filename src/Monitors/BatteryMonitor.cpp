#include "BatteryMonitor.hpp"

BatteryMonitor::BatteryMonitor(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::battery::pin, OUTPUT);
}

void BatteryMonitor::execute(){
    sfr::battery::voltage = analogRead(constants::battery::pin);
}

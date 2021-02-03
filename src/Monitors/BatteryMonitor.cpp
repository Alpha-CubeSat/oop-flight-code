#include "BatteryMonitor.hpp"

BatteryMonitor::BatteryMonitor(){
    pinMode(constants::battery::pin, OUTPUT);
}

void BatteryMonitor::execute(){
    sfr::battery::voltage = analogRead(constants::battery::pin);
}

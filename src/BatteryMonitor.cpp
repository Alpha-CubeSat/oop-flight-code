#include "BatteryMonitor.hpp"

BatteryMonitor::BatteryMonitor(){
    pinMode(constants::battery::pin, OUTPUT);
}

void BatteryMonitor::execute(){
    sfr::battery::voltage = map(analogRead(constants::battery::pin), 0, 1023, 0, 255);
}

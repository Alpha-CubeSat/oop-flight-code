#include "CurrentMonitor.hpp"

CurrentMonitor::CurrentMonitor(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::current::pin, OUTPUT);
}

void CurrentMonitor::execute(){
    uint16_t val = analogRead(constants::current::pin);
    float voltage = (val * constants::current::voltage_ref) / constants::current::resolution;
    float milliamps = 1000 * voltage / (constants::current::load * constants::current::shunt);

    sfr::current::solar_current = milliamps;

    sfr::current::solar_current_buffer.push_front(sfr::current::solar_current);
    if(sfr::current::solar_current_buffer.size() > constants::sensor::collect) {
        sfr::current::solar_current_buffer.pop_back();
    }
    float sum = std::accumulate(sfr::current::solar_current_buffer.begin(), sfr::current::solar_current_buffer.end(), (float) 0.0);
    sfr::current::solar_current_average = sum / sfr::current::solar_current_buffer.size();
    #ifdef VERBOSE
        Serial.print("Currrent: ");
        Serial.print(sfr::current::solar_current_average);
        Serial.println(" mA");
    #endif
    sfr::current::in_sun = sfr::current::solar_current_average >= constants::current::in_sun_val;
}
#include "CurrentMonitor.hpp"

CurrentMonitor::CurrentMonitor(unsigned int offset): TimedControlTask<void>(offset){
    pinMode(constants::current::pin, OUTPUT);
}

void CurrentMonitor::execute(){
    float val = analogRead(constants::current::pin);
    sfr::current::solar_current_buffer.push_front(val);
    sfr::current::solar_current = val;

    if(sfr::current::solar_current_buffer.size() > constants::sensor::collect) {
        sfr::current::solar_current_buffer.pop_back();
    }
    float sum = std::accumulate(sfr::current::solar_current_buffer.begin(), sfr::current::solar_current_buffer.end(), 0.0);
    sfr::current::solar_current_average = sum / sfr::current::solar_current_buffer.size();

    sfr::current::in_sun = sfr::current::solar_current_average >= constants::current::in_sun_val;
}
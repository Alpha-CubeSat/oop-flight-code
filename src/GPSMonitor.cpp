#include "GPSMonitor.hpp"

GPSMonitor::GPSMonitor(HardwareSerial ser):
    gps(),
    hwSer(ser){
    hwSer.begin(constants::gps::baud);
}

void GPSMonitor::execute(){
    if(hwSer.available() > 0){
        gps.encode(hwSer.read());
        sfr::gps::altitude = gps.altitude.feet();
    }
}
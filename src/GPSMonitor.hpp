#ifndef GPS_MONITOR_HPP_
#define GPS_MONITOR_HPP_

#include <TinyGPS++.h>    
#include "sfr.hpp"
#include "constants.hpp"

class GPSMonitor{
    public:
        GPSMonitor(HardwareSerial ser);
        void execute();
        TinyGPSPlus gps;
        HardwareSerial hwSer;
};

#endif
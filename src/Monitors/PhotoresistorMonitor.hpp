#ifndef PHOTORESISTOR_MONITOR_HPP_
#define PHOTORESISTOR_MONITOR_HPP_

#include "sfr.hpp"
#include "constants.hpp"
#include <Arduino.h>

class PhotoresistorMonitor{
    public:
        PhotoresistorMonitor();
        void execute();
};

#endif
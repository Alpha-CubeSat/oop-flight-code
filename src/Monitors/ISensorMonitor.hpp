#pragma once

#include "sfr.hpp"

class ISensorMonitor{
    public:
        virtual void execute()=0;
        virtual void transition_to_normal()=0;
        virtual void transition_to_abnormal()=0;
        virtual void transition_to_abandon()=0;
    protected:
        sensor_mode_type mode = sensor_mode_type::normal;
        bool check_sensor = true;
};
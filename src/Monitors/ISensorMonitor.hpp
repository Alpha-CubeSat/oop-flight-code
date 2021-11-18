#pragma once

#include "sfr.hpp"

class ISensorMonitor{
    public:
        virtual void execute()=0;
        virtual void transition_to_normal() = 0;
        virtual void transition_to_abnormal_init() = 0;
        virtual void transition_to_abnormal_readings() = 0;
        virtual void transition_to_retry() = 0;
        virtual void transition_to_abandon() = 0;
    protected:
        // sensor_mode_type mode = sensor_mode_type::normal;
        // bool check_sensor = true;
        // i think we decided these should be sfr values since they should be changeable by command for each sensor
};
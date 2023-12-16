#ifndef SETTING_HPP_
#define SETTING_HPP_

#include "Arduino.h"

typedef struct
{
 bool rockblock_sleep_mode;
 bool imu_power_setting;
 bool camera_power_setting;
 bool acs_off;
 uint32_t downlink_period;
}setting;


#endif
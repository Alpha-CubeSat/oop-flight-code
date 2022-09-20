#ifndef ACS_MONITOR_HPP_
#define ACS_MONITOR_HPP_

#include "DataLogging.hpp"
#include "sfr.hpp"

class ACSMonitor : public TimedControlTask<void>
{
public:
    ACSMonitor(unsigned int offset);
    void execute();
    void IMUOffset(float *mag_x, float *mag_y, float *mag_z, float temp, float voltage, float pwmX, float pwmY, float pwmZ);
    StarshotACSModelClass starshotObj;
};

#endif
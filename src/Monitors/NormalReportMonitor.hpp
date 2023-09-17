#ifndef NORMAL_REPORT_MONITOR_HPP_
#define NORMAL_REPORT_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "Faults.hpp"
#include <queue>

class NormalReportMonitor : public TimedControlTask<void>
{
public:
    NormalReportMonitor(unsigned int offset);
    void execute();
    uint8_t serialize(float value, float min, float max);
    uint8_t serialize(bool values[]);

private:
    float light_avg_standby;
    float light_avg_deploy;
    float mag_x;
    float mag_y;
    float mag_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float mag_x_avg;
    float mag_y_avg;
    float mag_z_avg;
    float gyro_x_avg;
    float gyro_y_avg;
    float gyro_z_avg;
    float temp;
    float temp_avg;
    float current_avg;
    float voltage;
    float voltage_avg;
};

#endif
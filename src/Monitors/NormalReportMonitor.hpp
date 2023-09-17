#ifndef NORMAL_REPORT_MONITOR_HPP_
#define NORMAL_REPORT_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "Faults.hpp"
#include "SFRField.hpp"
#include "SensorReading.hpp"
#include <queue>

class NormalReportMonitor : public TimedControlTask<void>
{
public:
    NormalReportMonitor(unsigned int offset);
    void execute();
    uint8_t serialize(SensorReading *valueObj);
    uint8_t serialize(SFRField valueObj);
    uint8_t serialize(float value, float min, float max);
    uint8_t serialize(bool values[]);
};

#endif
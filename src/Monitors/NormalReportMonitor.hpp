#ifndef NORMAL_REPORT_MONITOR_HPP_
#define NORMAL_REPORT_MONITOR_HPP_

#include "Faults.hpp"
#include "SFRField.hpp"
#include "SensorReading.hpp"
#include <queue>

class NormalReportMonitor
{
public:
    NormalReportMonitor();
    void execute();

private:
    uint8_t serialize(SensorReading *valueObj);
    uint8_t serialize(int opcode);
    uint8_t serialize(float value, float min, float max);
    uint8_t serialize(bool values[]);
};

#endif
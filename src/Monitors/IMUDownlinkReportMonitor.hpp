#ifndef IMU_DOWNLINK_REPORT_MONITOR_HPP_
#define IMU_DOWNLINK_REPORT_MONITOR_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "sfr.hpp"
#include <SD.h>
#include <cassert>
#include <vector>

class IMUDownlinkReportMonitor : public TimedControlTask<void>
{
public:
    IMUDownlinkReportMonitor(unsigned int offset);
    void execute();

private:
    void create_imu_downlink_report(int fragment_number);
    void write_imu_report_to_SD(int fragment_number);
    bool start_timing_deployed = false;
    uint8_t current_sample = 0;
    uint8_t fragment_number = 0;
    bool report_ready = false;
    bool report_downlinked = true;
    // Sets the amount of values that go into the report.
    int pop_size = min(constants::imu::max_gyro_imu_report_size, sfr::imu::imu_dlink.size());
    File imgFile;
};

#endif
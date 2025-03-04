#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "Adafruit_LSM9DS1.h"
#include "../../src/constants.hpp"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "Pins.hpp"
#include "SensorReading.hpp"
#include "ekf.h"
#include "sfr.hpp"
#include <map>
#include <cmath>

#ifdef ACS_SIM
#include "Plant.h"
#endif

using namespace constants::imu;

// Error codes
enum class EKFError {
    OK = 0,
    VOLTAGE_OUT_OF_BOUNDS = 1,
    INSUFFICIENT_CALIBRATION_DATA = 2,
    INTERPOLATION_ERROR = 3,
    OFFSET_CALCULATION_ERROR = 4,
    STATE_BOUNDS_EXCEEDED = 5
};

typedef std::array<std::array<float, 3>, 7> OffsetArray;

class IMUMonitor
{
public:
    EKF ekfObj;
#ifdef ACS_SIM
    Plant plantObj;
#endif

    IMUMonitor();
    void execute();

private:
    // Takes the precision beyond the tenths place of voltage and turns that into a percentage
    // that can then be factored and used to interpolate the values that are between the values
    // in the multidimensional arrays that have been generated from the values in the csv files.
    bool getInterpolatedOffsets(double voltage, PWMCoefficients& coeffs, 
                            const std::map<float, VoltageCoefficients>& voltage_coeffs,
                            const std::function<const PWMCoefficients&(const VoltageCoefficients&)>& coeffSelector);
    bool calculatePWMValues(double voltage, float pwm_x, float pwm_y, float pwm_z,
                       float& pwmX_ox, float& pwmX_oy, float& pwmX_oz,
                       float& pwmY_ox, float& pwmY_oy, float& pwmY_oz,
                       float& pwmZ_ox, float& pwmZ_oy, float& pwmZ_oz);
    void IMU_init();
    void invalidate_data();
    void transition_to_normal();
    void transition_to_abnormal_init();
    void capture_imu_values();
    void imu_offset();

    float temp;
    float voltage;

    float mag_x;
    float mag_y;
    float mag_z;

    float gyro_x;
    float gyro_y;
    float gyro_z;

    // Pre-allocated arrays for offset storage
    std::array<float, 3> x_offsets;
    std::array<float, 3> y_offsets;
    std::array<float, 3> z_offsets;

    Adafruit_LSM9DS1 imu;

    bool first = true;

#ifdef ACS_SIM
    double altitude_input = 400;
    double I_input[9] = {0.002102, -0.000049, 0.000011,
                         -0.000049, 0.002084, 0.000019,
                         0.000011, 0.000019, 0.002437};

    double inclination_input = 0.90058989402907408; // 51.6 deg in rad
    double m_input = 1.3;                           // kg
    double q0_input[4] = {0.5, 0.5, -0.18301270189221924, 0.6830127018922193};

    // detumble starting conditions (1 - 5 deg per second magnitude)
    double wx_input = 0.05;
    double wy_input = -0.04;
    double wz_input = -0.01;
#endif
};

#endif
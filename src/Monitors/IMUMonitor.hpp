#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "Adafruit_LSM9DS1.h"
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

// Structure to hold the three coefficients for each component
struct PWMCoefficients {
    float coeff_1;
    float coeff_2;
    float coeff_3;
};

    // Structure to hold ox, oy, oz coefficients for each PWM
struct PWMAxisCoefficients {
    PWMCoefficients ox;
    PWMCoefficients oy;
    PWMCoefficients oz;
};

    // Structure to hold all PWM coefficients for a specific voltage
struct VoltageCoefficients {
    PWMAxisCoefficients pwmX;
    PWMAxisCoefficients pwmY;
    PWMAxisCoefficients pwmZ;
};

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
    // Define constants surrounding interpolated data for function usage.
    static constexpr size_t VOLTAGE_LEVELS = 7;  // 3.6V to 4.2V
    static constexpr float BASE_VOLTAGE = 3.6;
    static constexpr float VOLTAGE_STEP = 0.1;
    static constexpr float MAX_VOLTAGE = 4.2;

    // Global map to store all coefficients indexed by voltage
    const std::map<float, VoltageCoefficients> voltage_coefficients = {
        {3.6f, {
            // PWM X coefficients for 3.6V
            {{0.052304164f, -2.61e-06f, -3.58e-08f},
             {-0.002711837f, 1.5211777986472764e-05f, 1.3891234756939779e-08f},
             {-0.012450927f, -3.34E-05f, 4.688569949391245e-08f}},
            // PWM Y coefficients for 3.6V
            {{-0.022939702f, -8.53E-07f, 6.232387253474929e-08f},
             {0.037193660324958876f, 8.335467713936068e-06f, -5.39E-08f},
             {0.049852505f, -3.99E-05f, 1.7167976102493515e-08f}},
            // PWM Z coefficients for 3.6V
            {{0.032277275f, 2.817714410389243e-06f, -5.25E-08f},
             {0.009254934f, 8.771497955737694e-06f, -2.07E-08f},
             {0.011231273082160992f, -2.72E-05f, 1.1955898748351396e-08f}}
        }},
        {3.7f, {
            // PWM X coefficients for 3.7V
            {{0.053660461f, 2.636434351195745e-06f, -3.79E-08f},
             {-0.001348294f, 4.8687593082709684e-06f, -1.65E-08f},
             {-0.008692989f, -4.14E-05f, -1.71E-08f}},
            // PWM Y coefficients for 3.7V
            {{-0.019415117f, -1.09E-06f, -2.87E-08f},
             {0.037740391f, 1.2707912386047763e-05f, -5.47E-08f},
             {0.054619152118486204f, -4.13E-05f, -4.94E-08f}},
            // PWM Z coefficients for 3.7V
            {{0.033473917670893256f, 1.1519821281197552e-05f, -6.51E-08f},
             {0.009280745f, 1.3079342041606201e-05f, -1.57E-08f},
             {0.012951321338668732f, -2.79E-05f, -1.69E-08f}}
        }},
        {3.8f, {
            // PWM X coefficients for 3.8V
            {{0.052363683157800794f, 5.029928759118574e-06f, -1.33E-08f},
             {-0.00062765f, 9.999241109119009e-06f, -6.81E-09f},
             {-0.005126981f, -3.98E-05f, -4.94E-08f}},
            // PWM Y coefficients for 3.8V
            {{-0.023484185f, 3.351451853116678e-06f, 3.8541061848498047e-08f},
             {0.041582463f, 1.5040316078051909e-05f, -7.82E-08f},
             {0.05913064f, -4.58E-05f, -1.02E-07f}},
            // PWM Z coefficients for 3.8V
            {{0.038240435090046626f, 3.02987184230247e-06f, -1.06E-07f},
             {0.012129896f, 1.0956012787311347e-05f, -4.68E-08f},
             {0.012532485105431838f, -3.21E-05f, -3.98E-09f}}
        }},
        {3.9f, {
            // PWM X coefficients for 3.9V
            {{0.060439794f, 7.037527154064302e-06f, -1.18E-07f},
             {-0.001709494f, 1.3578644810609342e-05f, -2.97E-08f},
             {-0.006518162f, -4.42E-05f, -6.01E-08f}},
            // PWM Y coefficients for 3.9V
            {{-0.022235625f, -8.11E-07f, 6.442112990392929e-09f},
             {0.03792121f, 1.8875798021191993e-05f, -5.31E-09f},
             {0.057770882f, -4.57E-05f, -6.77E-08f}},
            // PWM Z coefficients for 3.9V
            {{0.036998456f, 7.489304381646243e-06f, -5.86E-08f},
             {0.011103404f, 1.6232675944107693e-05f, -2.49E-08f},
             {0.010900746611262715f, -4.69E-05f, 4.841908615493492e-08f}}
        }},
        {4.0f, {
            // PWM X coefficients for 4.0V
            {{0.058274199f, 6.019475037233112e-06f, -6.33E-08f},
             {-0.002655724f, 2.8548526328769905e-06f, -1.83E-08f},
             {-0.006694983f, -5.29E-05f, -6.54E-08f}},
            // PWM Y coefficients for 4.0V
            {{-0.022613578f, -1.17E-06f, 1.4808926018693044e-08f},
             {0.039803983f, 1.5575523871861282e-05f, -2.40E-08f},
             {0.060711415f, -5.17E-05f, -4.07E-08f}},
            // PWM Z coefficients for 4.0V
            {{0.041155332f, 1.8051642524450853e-06f, -1.15E-07f},
             {0.009962505f, 1.1443789901059602e-05f, -1.34E-09f},
             {0.017596636680210484f, -3.27E-05f, -8.44E-08f}}
        }},
        {4.1f, {
            // PWM X coefficients for 4.1V
            {{0.06220939f, 2.415739396871471e-06f, -9.32E-08f},
             {-0.003212863f, 1.2659011354904793e-05f, 9.462933991235876e-09f},
             {-0.010880701f, -4.15E-05f, 3.526592205837461e-08f}},
            // PWM Y coefficients for 4.1V
            {{-0.023298604f, -5.04E-06f, 1.3903472283050426e-08f},
             {0.043592667f, 1.0214908411356832e-05f, -4.22E-08f},
             {0.061454786620047434f, -4.83E-05f, -3.95E-08f}},
            // PWM Z coefficients for 4.1V
            {{0.036823036f, 9.585787871026496e-06f, -2.66E-08f},
             {0.010660959f, 1.5953404099908e-05f, 6.88939867630209e-09f},
             {0.017761999370711913f, -3.68E-05f, -8.23E-08f}}
        }},
        {4.2f, {
            // PWM X coefficients for 4.2V
            {{0.059764578f, 2.767153305444127e-06f, -2.65E-08f},
             {-0.00260689f, 7.199123481032459e-06f, 3.048121361106912e-08f},
             {-0.00938585f, -4.73E-05f, 1.9678886493536774e-08f}},
            // PWM Y coefficients for 4.2V
            {{-0.02350586f, -2.26E-06f, -1.13E-08f},
             {0.044258641f, 2.9045552425130668e-05f, -9.49E-08f},
             {0.0609537f, -3.66E-05f, -3.25E-08f}},
            // PWM Z coefficients for 4.2V
            {{0.037040335f, 4.5172979690183935e-07f, -2.44E-08f},
             {0.010538882475480698f, 1.3995513057666222e-05f, 6.047514371598724e-09f},
             {0.014595059288760731f, -3.39E-05f, -4.06E-08f}}
        }}
    };

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
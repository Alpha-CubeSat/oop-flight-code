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
            {{0.048237582580201895f, 4.817059867004796e-07f, 1.2375342009082519e-08f},
             {-0.005414873365012105f, 1.37981065672519e-05f, 3.684230432288151e-08f},
             {-0.01141891526691749f, -4.1540643349744354e-05f, 4.413336488808188e-08f}},
            // PWM Y coefficients for 3.6V
            {{-0.021256780237274127f, -6.523378582202163e-06f, 5.1019026601712185e-08f},
             {0.03345060493007219f, 1.2289668649269109e-05f, 2.03553588681225e-08f},
             {0.05648824453283386f, -4.5763444226263266e-05f, -1.3013545421980455e-07f}},
            // PWM Z coefficients for 3.6V
            {{0.036627154514002454f, 5.162544940569339e-06f, -6.822404380672945e-08f},
             {0.012103438757156849f, 1.485291746113059e-05f, -1.85172660200408e-08f},
             {0.012681893730062435f, -3.7592513541459156e-05f, 1.908692696705999e-08f}}
        }},
        {3.7f, {
            // PWM X coefficients for 3.7V
            {{0.053821283323697316f, 1.9287686046842723e-06f, -5.713491507054209e-08f},
             {-0.001969173190233128f, 1.663104622594079e-05f, -2.154252209746119e-08f},
             {-0.01004306757800097f, -3.9204824648775786e-05f, 1.231583806500535e-08f}},
            // PWM Y coefficients for 3.7V
            {{-0.02011220054866004f, -4.0300900234307185e-06f, 2.7044522050071656e-08f},
             {0.03552975268760508f, 1.1826602919832653e-05f, -1.4036072859602733e-08f},
             {0.05671525857502551f, -5.271303489949437e-05f, -9.896536644594377e-08f}},
            // PWM Z coefficients for 3.7V
            {{0.03675622083962817f, 2.1483252226870454e-06f, -2.360930884793272e-08f},
             {0.008370500864285556f, 1.6975630116584584e-05f, 2.341276925072673e-08f},
             {0.014456553120287867f, -3.8075025849720677e-05f, -1.4578794822968068e-08f}}
        }},
        {3.8f, {
            // PWM X coefficients for 3.8V
            {{0.05750520036854666f, 3.596526176992339e-06f, -8.780199124261263e-08f},
             {-0.001962297219983119f, 1.6116707931358333e-05f, -1.317993881811886e-08f},
             {-0.009418240588079645f, -3.651360786210954e-05f, 1.763406979833158e-08f}},
            // PWM Y coefficients for 3.8V
            {{-0.022158995290071877f, 4.229868047847738e-07f, 3.250960409672966e-08f},
             {0.03718964926634184f, 1.158987639564781e-05f, -1.1303367574177954e-08f},
             {0.05531804342223322f, -4.443737727311534e-05f, -5.161012375218809e-08f}},
            // PWM Z coefficients for 3.8V
            {{0.041632325229134334f, 1.9600728535245935e-06f, -9.565380153615442e-08f},
             {0.010174517179289667f, 9.8278266313782e-06f, 2.3451863629443782e-08f},
             {0.01547777313121376f, -3.9518578597379884e-05f, -3.397556116757022e-08f}}
        }},
        {3.9f, {
            // PWM X coefficients for 3.9V
            {{0.05603131198866494f, 9.231006384169569e-06f, -3.6425899910916955e-08f},
             {-0.0031925267491582725f, 1.543835434512467e-05f, 1.122374152340878e-08f},
             {-0.010297805056989295f, -4.3279973818264597e-05f, -2.1874372595793425e-09f}},
            // PWM Y coefficients for 3.9V
            {{-0.021897254249224278f, 3.300701025450551e-07f, 1.745223165866891e-08f},
             {0.03732839306560172f, 1.6273608621000377e-05f, -7.895955731693936e-09f},
             {0.05795993725824469f, -4.316006905907015e-05f, -4.986681013318043e-08f}},
            // PWM Z coefficients for 3.9V
            {{0.04374482792765256f, 4.081552311297053e-06f, -1.2768819539074762e-07f},
             {0.010755022225277484f, 1.0027984101236037e-05f, 1.0009967834607427e-08f},
             {0.015324144449088948f, -3.4697771706650734e-05f, -1.7882351955603705e-08f}}
        }},
        {4.0f, {
            // PWM X coefficients for 4.0V
            {{0.05769967837995029f, 2.703975639602751e-06f, -4.575360513207314e-08f},
             {-0.004173134449716048f, 1.2326379995636406e-05f, 3.6817049025706574e-08f},
             {-0.011728339429921008f, -3.06100059762657e-05f, 1.987181232464245e-08f}},
            // PWM Y coefficients for 4.0V
            {{-0.018763418393995528f, -7.4053520779381305e-06f, -1.1229038975986821e-08f},
             {0.038144572427202816f, -1.278788051263085e-05f, 9.730484092304339e-08f},
             {0.05868914488315153f, -2.6526176992325682e-05f, -1.6930724144930518e-07f}},
            // PWM Z coefficients for 4.0V
            {{0.04140658475511416f, 3.8000986558145155e-06f, -6.888548619958153e-08f},
             {0.011825996802966823f, 1.0627318174488018e-05f, -2.0239143435591533e-08f},
             {0.01677266512621674f, -4.526841021846571e-05f, -4.335065600326439e-08f}}
        }},
        {4.1f, {
            // PWM X coefficients for 4.1V
            {{0.05721024388219172f, -1.8639782957207992e-06f, -3.890305444134525e-08f},
             {-0.003092790943179388f, 1.1017482948670489e-05f, 1.4744165687559536e-09f},
             {-0.012510903253855522f, -3.458986690951177e-05f, 2.8125284507082266e-08f}},
            // PWM Y coefficients for 4.1V
            {{-0.024286689023148497f, -2.955405674606547e-06f, 3.7171115972447666e-08f},
             {0.03823817229549751f, 1.961638065966585e-05f, 3.393580974047106e-08f},
             {0.06461898631848689f, -5.047098665300657e-05f, -1.0348717340948225e-07f}},
            // PWM Z coefficients for 4.1V
            {{0.03422425329325994f, 4.804965043588776e-06f, 4.9927012191163344e-08f},
             {0.007771106600268655f, 1.754987336008421e-05f, 1.50431637668039e-08f},
             {0.015258147165860826f, -3.4628001176280876e-05f, -6.616559335760512e-09f}}
        }},
        {4.2f, {
            // PWM X coefficients for 4.2V
            {{0.06342925759651621f, -8.914501456121884e-06f, -8.275212270772762e-08f},
             {-0.002713495100287549f, 1.0602464498135953e-05f, 2.2483374314672595e-08f},
             {-0.011118873958080392f, -4.22382063614028e-05f, 1.4083216080996287e-08f}},
            // PWM Y coefficients for 4.2V
            {{-0.024745671837514232f, -2.773082140451719e-06f, 1.8235967181582964e-08f},
             {0.03684671650542904f, 1.2165400267509016e-05f, 4.450763607256386e-08f},
             {0.06804203002674147f, -4.372610679491918e-05f, -1.403667652280303e-07f}},
            // PWM Z coefficients for 4.2V
            {{0.04586161869843892f, 3.782074997391328e-06f, -1.0634274674230334e-07f},
             {0.011648957425400274f, 1.6920752819753876e-05f, -1.1686837436005779e-09f},
             {0.012772575738890946f, -3.4978039595131755e-05f, 4.719668870612258e-08f}}
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
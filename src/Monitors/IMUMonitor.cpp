#include "IMUMonitor.hpp"

IMUMonitor::IMUMonitor()
{
    imu = Adafruit_LSM9DS1(constants::imu::CSAG, constants::imu::CSM);
}

void IMUMonitor::IMU_init()
{
    if (sfr::imu::init_mode == (uint16_t)sensor_init_mode_type::awaiting) {
        // Called imu_init function and initialization process has not yet started
        sfr::imu::init_mode = (uint16_t)sensor_init_mode_type::in_progress;
    }

    if (sfr::imu::init_mode == (uint16_t)sensor_init_mode_type::in_progress) {
        if (!imu.begin()) {
            sfr::imu::init_mode = (uint16_t)sensor_init_mode_type::failed;
        } else {
            sfr::imu::init_mode = (uint16_t)sensor_init_mode_type::complete;
            imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
            imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
            imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
        }
    }
}
bool IMUMonitor::getInterpolatedOffsets(double volt, PWMCoefficients& coeffs, 
                            const std::map<float, VoltageCoefficients>& voltage_coeffs,
                            const std::function<const PWMCoefficients&(const VoltageCoefficients&)>& coeffSelector) {
    // Validate voltage range
    if (volt < 3.6) {
        return false;
    }
    double voltage = volt;
    if (voltage > 4.2) {
        voltage = 4.2;
    }
    // Find the surrounding voltage levels
    auto upper_it = voltage_coeffs.upper_bound(static_cast<float>(voltage));
    if (upper_it == voltage_coeffs.begin() || upper_it == voltage_coeffs.end()) {
        return false;
    }
    auto lower_it = std::prev(upper_it);
    // Calculate interpolation factor
    float lower_voltage = lower_it->first;
    float upper_voltage = upper_it->first;
    float factor = static_cast<float>((voltage - lower_voltage) / (upper_voltage - lower_voltage));
    // Get coefficients for both voltage levels
    const PWMCoefficients& lower_coeffs = coeffSelector(lower_it->second);
    const PWMCoefficients& upper_coeffs = coeffSelector(upper_it->second);
    // Interpolate coefficients
    coeffs.coeff_1 = lower_coeffs.coeff_1 + factor * (upper_coeffs.coeff_1 - lower_coeffs.coeff_1);
    coeffs.coeff_2 = lower_coeffs.coeff_2 + factor * (upper_coeffs.coeff_2 - lower_coeffs.coeff_2);
    coeffs.coeff_3 = lower_coeffs.coeff_3 + factor * (upper_coeffs.coeff_3 - lower_coeffs.coeff_3);
    return true;
}

bool IMUMonitor::calculatePWMValues(double voltage, float pwm_x, float pwm_y, float pwm_z,
                       float& pwmX_ox, float& pwmX_oy, float& pwmX_oz,
                       float& pwmY_ox, float& pwmY_oy, float& pwmY_oz,
                       float& pwmZ_ox, float& pwmZ_oy, float& pwmZ_oz) {
    PWMCoefficients x_ox_coeffs, x_oy_coeffs, x_oz_coeffs;
    PWMCoefficients y_ox_coeffs, y_oy_coeffs, y_oz_coeffs;
    PWMCoefficients z_ox_coeffs, z_oy_coeffs, z_oz_coeffs;
    bool success = true;
    // Get interpolated coefficients for PWM X
    success &= getInterpolatedOffsets(voltage, x_ox_coeffs, voltage_coefficients,
        [](const VoltageCoefficients& vc) -> const PWMCoefficients& { return vc.pwmX.ox; });
    success &= getInterpolatedOffsets(voltage, x_oy_coeffs, voltage_coefficients,
        [](const VoltageCoefficients& vc) -> const PWMCoefficients& { return vc.pwmX.oy; });
    success &= getInterpolatedOffsets(voltage, x_oz_coeffs, voltage_coefficients,
        [](const VoltageCoefficients& vc) -> const PWMCoefficients& { return vc.pwmX.oz; });
    // Get interpolated coefficients for PWM Y
    success &= getInterpolatedOffsets(voltage, y_ox_coeffs, voltage_coefficients,
        [](const VoltageCoefficients& vc) -> const PWMCoefficients& { return vc.pwmY.ox; });
    success &= getInterpolatedOffsets(voltage, y_oy_coeffs, voltage_coefficients,
        [](const VoltageCoefficients& vc) -> const PWMCoefficients& { return vc.pwmY.oy; });
    success &= getInterpolatedOffsets(voltage, y_oz_coeffs, voltage_coefficients,
        [](const VoltageCoefficients& vc) -> const PWMCoefficients& { return vc.pwmY.oz; });
    // Get interpolated coefficients for PWM Z
    success &= getInterpolatedOffsets(voltage, z_ox_coeffs, voltage_coefficients,
        [](const VoltageCoefficients& vc) -> const PWMCoefficients& { return vc.pwmZ.ox; });
    success &= getInterpolatedOffsets(voltage, z_oy_coeffs, voltage_coefficients,
        [](const VoltageCoefficients& vc) -> const PWMCoefficients& { return vc.pwmZ.oy; });
    success &= getInterpolatedOffsets(voltage, z_oz_coeffs, voltage_coefficients,
        [](const VoltageCoefficients& vc) -> const PWMCoefficients& { return vc.pwmZ.oz; });
    // Calculate PWM X values using interpolated coefficients
    pwmX_ox = x_ox_coeffs.coeff_1 * pwm_x + x_ox_coeffs.coeff_2 * pow(pwm_x, 2) + x_ox_coeffs.coeff_3 * pow(pwm_x, 3);
    pwmX_oy = x_oy_coeffs.coeff_1 * pwm_x + x_oy_coeffs.coeff_2 * pow(pwm_x, 2) + x_oy_coeffs.coeff_3 * pow(pwm_x, 3);
    pwmX_oz = x_oz_coeffs.coeff_1 * pwm_x + x_oz_coeffs.coeff_2 * pow(pwm_x, 2) + x_oz_coeffs.coeff_3 * pow(pwm_x, 3);
    // Calculate PWM Y values using interpolated coefficients
    pwmY_ox = y_ox_coeffs.coeff_1 * pwm_y + y_ox_coeffs.coeff_2 * pow(pwm_y, 2) + y_ox_coeffs.coeff_3 * pow(pwm_y, 3);
    pwmY_oy = y_oy_coeffs.coeff_1 * pwm_y + y_oy_coeffs.coeff_2 * pow(pwm_y, 2) + y_oy_coeffs.coeff_3 * pow(pwm_y, 3);
    pwmY_oz = y_oz_coeffs.coeff_1 * pwm_y + y_oz_coeffs.coeff_2 * pow(pwm_y, 2) + y_oz_coeffs.coeff_3 * pow(pwm_y, 3);
    // Calculate PWM Z values using interpolated coefficients
    pwmZ_ox = z_ox_coeffs.coeff_1 * pwm_z + z_ox_coeffs.coeff_2 * pow(pwm_z, 2) + z_ox_coeffs.coeff_3 * pow(pwm_z, 3);
    pwmZ_oy = z_oy_coeffs.coeff_1 * pwm_z + z_oy_coeffs.coeff_2 * pow(pwm_z, 2) + z_oy_coeffs.coeff_3 * pow(pwm_z, 3);
    pwmZ_oz = z_oz_coeffs.coeff_1 * pwm_z + z_oz_coeffs.coeff_2 * pow(pwm_z, 2) + z_oz_coeffs.coeff_3 * pow(pwm_z, 3);
    return success;
}

void IMUMonitor::execute()
{
    // handle latent turn on / turn off variables
    if (sfr::imu::power_setting == (uint8_t)sensor_power_mode_type::off && sfr::imu::powered == false) {
        sfr::imu::power_setting = (uint8_t)sensor_power_mode_type::do_nothing;
        invalidate_data();
        sfr::imu::failed_times = 0;
    }
    if (sfr::imu::power_setting == (uint8_t)sensor_power_mode_type::on && sfr::imu::powered == true) {
        sfr::imu::power_setting = (uint8_t)sensor_power_mode_type::do_nothing;
    }

    if (sfr::imu::power_setting == (uint8_t)sensor_power_mode_type::on && sfr::imu::powered == false) {
#ifdef VERBOSE
        Serial.println("Turned on IMU");
#endif
        sfr::imu::init_mode = (uint16_t)sensor_init_mode_type::awaiting;
        IMU_init();
        if (sfr::imu::init_mode == (uint16_t)sensor_init_mode_type::complete) {
            sfr::imu::power_setting = (uint8_t)sensor_power_mode_type::do_nothing;
            transition_to_normal();
            sfr::imu::powered = true;
        } else {
            if (sfr::imu::failed_times == sfr::imu::failed_limit) {
                transition_to_abnormal_init();
            } else {
                sfr::imu::failed_times = sfr::imu::failed_times + 1;
                sfr::imu::init_mode = (uint16_t)sensor_init_mode_type::awaiting;
            }
        }
    }

    if (sfr::imu::power_setting == (uint8_t)sensor_power_mode_type::off && sfr::imu::powered == true) {
#ifdef VERBOSE
        Serial.println("Turned off IMU");
#endif
        imu.shutdown();
        sfr::imu::powered = false;
        sfr::imu::power_setting = (uint8_t)sensor_power_mode_type::do_nothing;
        invalidate_data();

        // reset number of failed imu initialization attempts every time IMU is turned off
        sfr::imu::failed_times = 0;
    }

    if (sfr::imu::powered == true) {
#ifdef VERBOSE
        Serial.println("IMU is on");
#endif
        capture_imu_values();
    }
}

void IMUMonitor::transition_to_normal()
{
    // updates imu mode to normal
    // faults are cleared
    // all check flags are set to true
    sfr::imu::mode = (uint16_t)sensor_mode_type::normal;

    fault_groups::imu_faults::mag_x_average->restore();
    fault_groups::imu_faults::mag_y_average->restore();
    fault_groups::imu_faults::mag_z_average->restore();
    fault_groups::imu_faults::gyro_x_average->restore();
    fault_groups::imu_faults::gyro_y_average->restore();
    fault_groups::imu_faults::gyro_z_average->restore();

    fault_groups::imu_faults::mag_x_value->restore();
    fault_groups::imu_faults::mag_y_value->restore();
    fault_groups::imu_faults::mag_z_value->restore();
    fault_groups::imu_faults::gyro_x_value->restore();
    fault_groups::imu_faults::gyro_y_value->restore();
    fault_groups::imu_faults::gyro_z_value->restore();
}

void IMUMonitor::transition_to_abnormal_init()
{
    // updates imu mode to abnormal_init
    // trips fault
    // all check flags are set to false
    sfr::imu::mode = (uint16_t)sensor_mode_type::abnormal_init;
    invalidate_data();

    sfr::imu::power_setting = (uint8_t)sensor_power_mode_type::do_nothing;
    sfr::imu::powered = false;
    sfr::imu::init_mode = (uint16_t)sensor_init_mode_type::awaiting;
}

void IMUMonitor::invalidate_data()
{
    fault_groups::imu_faults::mag_x_average->force();
    fault_groups::imu_faults::mag_y_average->force();
    fault_groups::imu_faults::mag_z_average->force();
    fault_groups::imu_faults::gyro_x_average->force();
    fault_groups::imu_faults::gyro_y_average->force();
    fault_groups::imu_faults::gyro_z_average->force();

    fault_groups::imu_faults::mag_x_value->force();
    fault_groups::imu_faults::mag_y_value->force();
    fault_groups::imu_faults::mag_z_value->force();
    fault_groups::imu_faults::gyro_x_value->force();
    fault_groups::imu_faults::gyro_y_value->force();
    fault_groups::imu_faults::gyro_z_value->force();
}

void IMUMonitor::imu_offset()
{

    using namespace constants::acs;

    if (!sfr::temperature::temp_c_value->get_value(&temp)) {
        temp = 0;
    }

    if (!sfr::battery::voltage_value->get_value(&voltage)) {
        voltage = 0;
    }

    if (!sfr::imu::mag_x_value->get_value(&mag_x)) {
        mag_x = 0;
    }

    if (!sfr::imu::mag_y_value->get_value(&mag_y)) {
        mag_y = 0;
    }

    if (!sfr::imu::mag_z_value->get_value(&mag_z)) {
        mag_z = 0;
    }

    if (!sfr::imu::gyro_x_value->get_value(&gyro_x)) {
        gyro_x = 0;
    }

    if (!sfr::imu::gyro_y_value->get_value(&gyro_y)) {
        gyro_y = 0;
    }

    if (!sfr::imu::gyro_z_value->get_value(&gyro_z)) {
        gyro_z = 0;
    }
    Serial.println("Raw IMU Data:");
    Serial.print("Mag X: "); Serial.println(mag_x);
    Serial.print("Mag Y: "); Serial.println(mag_y);
    Serial.print("Mag Z: "); Serial.println(mag_z);
    Serial.print("Gyro X: "); Serial.println(gyro_x);
    Serial.print("Gyro Y: "); Serial.println(gyro_y);
    Serial.print("Gyro Z: "); Serial.println(gyro_z);
    Serial.println("----------");

    /*Offset Contributions from PWM (ex: pwmX_oX is contribution of X mag to offset x)*/
    float pwmX_ox, pwmX_oy, pwmX_oz;
    float pwmY_ox, pwmY_oy, pwmY_oz;
    float pwmZ_ox, pwmZ_oy, pwmZ_oz;

    calculatePWMValues(voltage, sfr::acs::pwm_x,    sfr::acs::pwm_y,    sfr::acs::pwm_z, 
                                pwmX_ox,            pwmX_oy,            pwmX_oz, 
                                pwmY_ox,            pwmY_oy,            pwmY_oz, 
                                pwmZ_ox,            pwmZ_oy,            pwmZ_oz
    );
    /*******************************************/
    /*Voltage Adjustment Coefficients */

    float Volt_c = 0.36432835 * voltage - 0.44007538;
    /*******************************************/
    /*Temperature Offset Terms*/
    // just so to get imu temp
    sensors_event_t accel, mag, gyro, temp_imu;
    imu.getEvent(&accel, &mag, &gyro, &temp_imu);

    float temp_x = (-0.06579) * temp_imu.temperature + 1.588;
    float temp_y = (0.0715) * temp_imu.temperature + (-2.023);// do not modify, add these values to the offset
    float temp_z = (0.206) * temp_imu.temperature + (-6.835);
    /*******************************************/
    /*Total Offsets*/
    
    float mag_xoffset = (pwmX_ox + pwmY_ox + pwmZ_ox) * Volt_c + temp_x + hardiron_x;// rewrite
    float mag_yoffset = (pwmX_oy + pwmY_oy + pwmZ_oy) * Volt_c + temp_y + hardiron_y;
    float mag_zoffset = (pwmX_oz + pwmY_oz + pwmZ_oz) * Volt_c + temp_z + hardiron_z;// rewrite this

    Serial.println("Hard Iron Components: ");
    Serial.print("Mag X: "); Serial.println(hardiron_x);
    Serial.print("Mag Y: "); Serial.println(hardiron_y);
    Serial.print("Mag Z: "); Serial.println(hardiron_z);
    Serial.println("________");

    /*******************************************/
    /* Finally, adjust magnetometer/gyro readings*/
    // Serial.println("Unfiltered:");
    // Serial.print("Mag X: "); Serial.println(mag_x);
    // Serial.print("Mag Y: "); Serial.println(mag_y);
    // Serial.print("Mag Z: "); Serial.println(mag_z);

    // Serial.print("Gyro X: "); Serial.println(gyro_x);
    // Serial.print("Gyro Y: "); Serial.println(gyro_y);
    // Serial.print("Gyro Z: "); Serial.println(gyro_z);
    float mag_x_corrected = mag_x - mag_xoffset;
    float mag_y_corrected = mag_y - mag_yoffset;
    float mag_z_corrected = mag_z - mag_zoffset;

    float gyro_x_corrected = -(gyro_x - (-0.02297));
    float gyro_y_corrected = gyro_y - (0.03015);
    float gyro_z_corrected = gyro_z - (-0.01396);

    sfr::imu::mag_x_value->set_value(mag_x_corrected);
    sfr::imu::mag_y_value->set_value(mag_y_corrected);
    sfr::imu::mag_z_value->set_value(mag_z_corrected);

    sfr::imu::gyro_x_value->set_value(gyro_x_corrected);
    sfr::imu::gyro_y_value->set_value(gyro_y_corrected);
    sfr::imu::gyro_z_value->set_value(gyro_z_corrected);
    Serial.println("Soft-Iron Filtered: ");
    Serial.print("Mag X: "); Serial.println(mag_x_corrected);
    Serial.print("Mag Y: "); Serial.println(mag_y_corrected);
    Serial.print("Mag Z: "); Serial.println(mag_z_corrected);

    Serial.print("Gyro X: "); Serial.println(gyro_x_corrected);
    Serial.print("Gyro Y: "); Serial.println(gyro_y_corrected);
    Serial.print("Gyro Z: "); Serial.println(gyro_z_corrected);
    Serial.print("_________");
}

// generate a normal random variable using Box-Muller transform
float generateGaussian(float mu, float sigma)
{
    float u1, u2;

    int seed1 = analogRead(10);
    int seed2 = analogRead(10);
    // generate two 'indepdendent' uniform random variables
    randomSeed(seed1);
    do {
        u1 = random(1000000) / 1000000.0;
    } while (u1 == 0);

    randomSeed(seed2);
    u2 = random(1000000) / 1000000.0;

    float mag = sqrt(-2.0 * log(u1));
    float z0 = mag * cos(2 * PI * u2) + mu;
    // float z1 = mag * sin(2 * PI * u2) + mu;
    return z0 * sigma + mu;
}

void IMUMonitor::capture_imu_values()
{
    sensors_event_t accel, mag, gyro, temp;
    imu.getEvent(&accel, &mag, &gyro, &temp);

    // IMU PRINT STATEMENTS FOR LOGGING AND GRAPHING IMU DATA
#ifdef IMU_TESTING

    Serial.print(millis());

    Serial.print(", ");
    Serial.print(mag.magnetic.x);
    Serial.print(", ");
    Serial.print(mag.magnetic.y);
    Serial.print(", ");
    Serial.print(mag.magnetic.z);
    Serial.print(", ");
    Serial.print(gyro.gyro.x);
    Serial.print(", ");
    Serial.print(gyro.gyro.y);
    Serial.print(", ");
    Serial.print(gyro.gyro.z);
    Serial.print(", ");

#endif

    // Save most recent readings
    sfr::imu::mag_x_value->set_value(mag.magnetic.x);
    sfr::imu::mag_y_value->set_value(mag.magnetic.y);
    sfr::imu::mag_z_value->set_value(mag.magnetic.z);

    sfr::imu::gyro_x_value->set_value(gyro.gyro.x);
    sfr::imu::gyro_y_value->set_value(gyro.gyro.y);
    sfr::imu::gyro_z_value->set_value(gyro.gyro.z);

    if (first) {
#ifdef VERBOSE
        Serial.println("Initialize EKF library");
#endif
        Eigen::VectorXd initial_state = Eigen::VectorXd::Zero(6);
        Eigen::MatrixXd initial_cov = Eigen::MatrixXd::Zero(6, 6);
        // Q (process noise covariance) Matrix
        Eigen::MatrixXd Q = 0.02 * Eigen::MatrixXd::Identity(6, 6);
        Q.diagonal() << 0.008, 0.07, 0.005, 0.1, 0.1, 0.1;
        // Rd (measurement noise variance) Matrices
        Eigen::MatrixXd Rd(6, 6);
        Rd << 2.02559220e-01, 5.17515015e-03, -3.16669361e-02, -1.76503506e-04, -3.74891174e-05, -7.75657503e-05,
            5.17515015e-03, 1.55389381e-01, 1.07780468e-02, -2.90511952e-05, -8.02931174e-06, -1.26277622e-05,
            -3.16669361e-02, 1.07780468e-02, 3.93162684e-01, 9.29630074e-05, 1.22496815e-05, 5.67092127e-05,
            -1.76503506e-04, -2.90511952e-05, 9.29630074e-05, 1.80161545e-05, -2.27002599e-09, -6.07376965e-07,
            -3.74891174e-05, -8.02931174e-06, 1.22496815e-05, -2.27002599e-09, 6.70144060e-06, 2.97298687e-08,
            -7.75657503e-05, -1.26277622e-05, 5.67092127e-05, -6.07376965e-07, 2.97298687e-08, 8.52192033e-06;
        // Hd
        Eigen::MatrixXd Hd = Eigen::MatrixXd::Identity(6, 6);

        ekfObj.initialize(constants::acs::step_size_input, initial_state, initial_cov, Q, Rd, Hd); // initialize with voltage
#ifdef ACS_SIM
        plantObj.initialize(0.01, altitude_input, I_input, inclination_input, m_input, q0_input, wx_input, wy_input, wz_input);
#endif
        first = false;
    }

    // offset the mag/gyro values in the sfr
    imu_offset();

// if ACS_SIM, plant will overwrite the sensor values to the sfr
#ifdef ACS_SIM

    // 1. Pass output of starshot into plant
    for (int i = 0; i < (int)(constants::acs::step_size_input / 0.01); i++) {
        plantObj.rtU.current[0] = sfr::acs::current_x;
        plantObj.rtU.current[1] = sfr::acs::current_y;
        plantObj.rtU.current[2] = sfr::acs::current_z;

        plantObj.step();
    }

    float mag_x_noise = generateGaussian(0.0, 2.02559220e-01);
    float mag_y_noise = generateGaussian(0.0, 1.55389381e-01);
    float mag_z_noise = generateGaussian(0.0, 3.93162684e-01);

    float gyro_x_noise = generateGaussian(0.0, 1.80161545e-05);
    float gyro_y_noise = generateGaussian(0.0, 6.70144060e-06);
    float gyro_z_noise = generateGaussian(0.0, 8.52192033e-06);

    sfr::imu::gyro_x_value->set_value(plantObj.rtY.angularvelocity[0] + gyro_x_noise);
    sfr::imu::gyro_y_value->set_value(plantObj.rtY.angularvelocity[1] + gyro_y_noise);
    sfr::imu::gyro_z_value->set_value(plantObj.rtY.angularvelocity[2] + gyro_z_noise);

    // Convert to uT

    sfr::imu::mag_x_value->set_value(plantObj.rtY.magneticfield[0] * 1000000.0 + mag_x_noise);
    sfr::imu::mag_y_value->set_value(plantObj.rtY.magneticfield[1] * 1000000.0 + mag_y_noise);
    sfr::imu::mag_z_value->set_value(plantObj.rtY.magneticfield[2] * 1000000.0 + mag_z_noise);

#endif

    // read imu data from sfr as local

    if (!sfr::imu::mag_x_value->get_value(&mag_x)) {
        mag_x = 0;
    }

    if (!sfr::imu::mag_y_value->get_value(&mag_y)) {
        mag_y = 0;
    }

    if (!sfr::imu::mag_z_value->get_value(&mag_z)) {
        mag_z = 0;
    }

    if (!sfr::imu::gyro_x_value->get_value(&gyro_x)) {
        gyro_x = 0;
    }

    if (!sfr::imu::gyro_y_value->get_value(&gyro_y)) {
        gyro_y = 0;
    }

    if (!sfr::imu::gyro_z_value->get_value(&gyro_z)) {
        gyro_z = 0;
    }

    //  Pass sensor data / output of plant into ekf
    ekfObj.Z(0) = mag_x;
    ekfObj.Z(1) = mag_y;
    ekfObj.Z(2) = mag_z;
    ekfObj.Z(3) = gyro_x;
    ekfObj.Z(4) = gyro_y;
    ekfObj.Z(5) = gyro_z;

    ekfObj.step();

#ifdef IMU_TESTING
    Serial.print(ekfObj.state(0));
    Serial.print(", ");
    Serial.print(ekfObj.state(1));
    Serial.print(", ");
    Serial.print(ekfObj.state(2));
    Serial.print(", ");
    Serial.print(ekfObj.state(3));
    Serial.print(", ");
    Serial.print(ekfObj.state(4));
    Serial.print(", ");
    Serial.println(ekfObj.state(5));
#endif
    // update the EKFed values
    Serial.println("EKF Filtered:");
    Serial.print("Mag X: "); Serial.println(ekfObj.state(0));
    Serial.print("Mag Y: ");Serial.println(ekfObj.state(1));
    Serial.print("Mag Z: "); Serial.println(ekfObj.state(2));
    Serial.print("Gyro X: "); Serial.println(ekfObj.state(3));
    Serial.print("Gyro Y: "); Serial.println(ekfObj.state(4));
    Serial.print("Gyro Z: "); Serial.println(ekfObj.state(5));
    Serial.println("_______");

    sfr::imu::mag_x_value->set_value(ekfObj.state(0));
    sfr::imu::mag_y_value->set_value(ekfObj.state(1));
    sfr::imu::mag_z_value->set_value(ekfObj.state(2));
    sfr::imu::gyro_x_value->set_value(ekfObj.state(3));
    sfr::imu::gyro_y_value->set_value(ekfObj.state(4));
    sfr::imu::gyro_z_value->set_value(ekfObj.state(5));

    // Add offset readings to buffer
    sfr::imu::mag_x_average->set_value(ekfObj.state(0));
    sfr::imu::mag_y_average->set_value(ekfObj.state(1));
    sfr::imu::mag_z_average->set_value(ekfObj.state(2));
    // used outside of ACS Control Task to determine exit conditions for Detumble Spin
    sfr::imu::gyro_x_average->set_value(ekfObj.state(3));
    sfr::imu::gyro_y_average->set_value(ekfObj.state(4));
    sfr::imu::gyro_z_average->set_value(ekfObj.state(5));
}

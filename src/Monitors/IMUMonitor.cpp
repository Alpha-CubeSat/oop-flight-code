#include "IMUMonitor.hpp"
// eliminate virtual classes and camera monitor.cpp
// adapt to new initialization process
// if in case 2 for two minutes, it has failed initialization

IMUMonitor::IMUMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    imu = Adafruit_LSM9DS1(constants::imu::CSAG, constants::imu::CSM);

    if (!imu.begin()) {
        sfr::imu::successful_init = false;
    } else {
        sfr::imu::successful_init = true;
        imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
        imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
        imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
    }
    sfr::imu::mode = sensor_mode_type::init; // This is already done in sfr.cpp but included for clarity
}

void IMUMonitor::execute()
{
    if (sfr::imu::sample) {
        switch (sfr::imu::mode) {
        case sensor_mode_type::init:
            // Reads result of IMUMonitor initialization and makes the transition to normal or abnormal_init.
            // This step is needed to wait for sfr.cpp to finish initalizing everything. The SensorReading object
            // constructors in sfr.cpp are not yet called when attempting to use those objects in the IMUMonitor
            // initialization.
            if (sfr::imu::successful_init) {
                transition_to_normal();
            } else {
                transition_to_abnormal_init();
            }
            break;
        case sensor_mode_type::normal:
#ifdef VERBOSE
            Serial.println("IMU is in Normal Mode");
#endif
            capture_imu_values();
            break;
        case sensor_mode_type::abnormal_init:
#ifdef VERBOSE
            Serial.println("IMU is in Abnormal Initialization Mode");
#endif
            break;
        case sensor_mode_type::retry:
#ifdef VERBOSE
            Serial.println("IMU is in Retry Mode");
#endif
            if (!imu.begin()) {
                transition_to_abnormal_init();
            } else {
                transition_to_normal();
                imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
                imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
                imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
            }
            break;
        }
    }
}

bool check_repeated_values(std::deque<float> buffer)
{
    if (buffer.empty() || buffer.size() == 1) {
        return false;
    }

    float first_val = buffer[0];
    for (float val : buffer) {
        if (first_val != val) {
            return false;
        }
    }

    return true;
}

// float x_new = 0;
// float x_old = 0;
// float x_oldLP = 0;
// float x_newLP = 0;

// float y_new = 0;
// float y_old = 0;
// float y_oldLP = 0;
// float y_newLP = 0;

// float z_new = 0;
// float z_old = 0;
// float z_oldLP = 0;
// float z_newLP = 0;

// //6th order low pass filter, cutoff frequency of 0.1 Hz
// float x_a = 0.98473518;
// float x_b[] = {0.00763241, 0.00763241};

// float LP_filter(float old_LPval, float new_val, float old_val, float a, float b[])
// {
//     float new_LPval = a * old_LPval + b[0] * new_val + b[1] * old_val;
//     return new_LPval;
// }

//Epanechnikov kernel for smooth
// return the weight for the given t=|x-x_0|/window_size
float Ep_kernel(float t)
{
    if (fabs(t) <= 1) {
        return 0.75*(1 -  pow(t, 2));
    } else {
        return 0;
    }
}

// Epanechnikov kernel smoothing
// return the smoothed value of the current window
float smooth_func(std::deque<float> buffer)
{
    int window_size = buffer.size();
    int x_0 = window_size / 2;
    float weight = 0;
    float weight_data_sum = 0;
    float weight_sum = 0;
    //Serial.print("Window Size: ");
    //Serial.println(window_size);
    for (int i = 0; i < window_size; i++) {
        weight = Ep_kernel(fabs(i - x_0) / window_size);
        Serial.print(String(i) +": raw buffer value: ");
        Serial.println(buffer.at(i));
        weight_data_sum += weight * buffer.at(i);
        weight_sum += weight;
    }
    Serial.println("Epa_smooth value: ");
    Serial.println(weight_data_sum / weight_sum);
    return weight_data_sum / weight_sum;
}
void IMUMonitor::capture_imu_values()
{
    sensors_event_t accel, mag, gyro, temp;
    
    imu.getEvent(&accel, &mag, &gyro, &temp);

    imu.setupMag(imu.LSM9DS1_MAGGAIN_8GAUSS);
    imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);

    sfr::imu::mag_x = mag.magnetic.x;
    sfr::imu::mag_y = mag.magnetic.y;
    sfr::imu::mag_z = mag.magnetic.z;

    sfr::imu::gyro_x = gyro.gyro.x;
    sfr::imu::gyro_y = gyro.gyro.y;
    sfr::imu::gyro_z = gyro.gyro.z;

    sfr::imu::gyro_x_value->set_value(sfr::imu::gyro_x);
    sfr::imu::gyro_y_value->set_value(sfr::imu::gyro_y);
    sfr::imu::gyro_z_value->set_value(sfr::imu::gyro_z);

    // Add reading(s) to buffer
    int collection_cycle = 3;

    for (int i = 0; i < collection_cycle; i++) {
        imu.getEvent(&accel, &mag, &gyro, &temp);
        // Serial.print("Mag timestamp: ");
        // Serial.println(mag.timestamp);

        sfr::imu::mag_x_buffer.push_front(mag.magnetic.x);
        sfr::imu::gyro_x_buffer.push_front(gyro.gyro.x);


        sfr::imu::mag_y_buffer.push_front(mag.magnetic.y);
        sfr::imu::gyro_y_buffer.push_front(gyro.gyro.y);

        sfr::imu::mag_z_buffer.push_front(mag.magnetic.z);
        sfr::imu::gyro_z_buffer.push_front(gyro.gyro.z);
        //delay for imu timestamp updating in order to get new data
        delay(2);
    }

    // Remove old readings

    if (sfr::imu::mag_x_buffer.size() > constants::sensor::collect) {
        for (int i = 0; i < collection_cycle; i++) {
            sfr::imu::mag_x_buffer.pop_back();
        }
    }
    if (sfr::imu::mag_y_buffer.size() > constants::sensor::collect) {
        for (int i = 0; i < collection_cycle; i++) {
            sfr::imu::mag_y_buffer.pop_back();
        }
    }
    if (sfr::imu::mag_z_buffer.size() > constants::sensor::collect) {
        for (int i = 0; i < collection_cycle; i++) {
            sfr::imu::mag_z_buffer.pop_back();
        }
    }
    if (sfr::imu::gyro_x_buffer.size() > constants::sensor::collect) {
        for (int i = 0; i < collection_cycle; i++) {
            sfr::imu::gyro_x_buffer.pop_back();
        }
    }
    if (sfr::imu::gyro_y_buffer.size() > constants::sensor::collect) {
        for (int i = 0; i < collection_cycle; i++) {
            sfr::imu::gyro_y_buffer.pop_back();
        }
    }
    if (sfr::imu::gyro_z_buffer.size() > constants::sensor::collect) {
        for (int i = 0; i < collection_cycle; i++) {
            sfr::imu::gyro_z_buffer.pop_back();
        }
    }


    // Check for repeated/invalid readings, calculate sums and average

    // if (check_repeated_values(sfr::imu::mag_x_buffer)) {
    //     sfr::imu::mag_x_average->set_invalid();
    // } else {
    //     sfr::imu::mag_x_average->set_valid();
    //     sfr::imu::mag_x_average->set_value(Epa_smooth(sfr::imu::mag_x_buffer));
    //     //float mag_x_sum = std::accumulate(sfr::imu::mag_x_buffer.begin(), sfr::imu::mag_x_buffer.end(), 0.0);
    //     //sfr::imu::mag_x_average->set_value(mag_x_sum / sfr::imu::mag_x_buffer.size());
    // }

    // sfr::imu::mag_x_average->set_valid();
    // sfr::imu::mag_y_average->set_valid();
    // sfr::imu::mag_z_average->set_valid();
    
    // sfr::imu::mag_x_average->set_value(smooth_func(sfr::imu::mag_x_buffer));
    // sfr::imu::mag_y_average->set_value(smooth_func(sfr::imu::mag_y_buffer));
    // sfr::imu::mag_z_average->set_value(smooth_func(sfr::imu::mag_z_buffer));

    // x_new = sfr::imu::mag_x;
    // x_newLP = LP_filter(x_oldLP, x_new, x_old, x_a, x_b);

    // y_new = sfr::imu::mag_y;
    // y_newLP = LP_filter(y_oldLP, y_new, y_old, x_a, x_b);

    // z_new = sfr::imu::mag_z;
    // z_newLP = LP_filter(z_oldLP, z_new, z_old, x_a, x_b);

    // sfr::imu::mag_x_average->set_value(x_newLP);
    // sfr::imu::mag_y_average->set_value(y_newLP);
    // sfr::imu::mag_z_average->set_value(z_newLP);

    // sfr::imu::mag_x_average->set_valid();
    // sfr::imu::mag_y_average->set_valid();
    // sfr::imu::mag_z_average->set_valid();

    // x_old = x_new;
    // x_oldLP = x_newLP;
    // y_old = y_new;
    // y_oldLP = y_newLP;
    // z_old = z_new;
    // z_oldLP = z_newLP;

    // if (check_repeated_values(sfr::imu::mag_y_buffer)) {
    //     sfr::imu::mag_y_average->set_invalid();
    // } else {
    //     sfr::imu::mag_y_average->set_valid();
    //     sfr::imu::mag_y_average->set_value(Epa_smooth(sfr::imu::mag_y_buffer));
    //     //float mag_y_sum = std::accumulate(sfr::imu::mag_y_buffer.begin(), sfr::imu::mag_y_buffer.end(), 0.0);
    //     //sfr::imu::mag_y_average->set_value(mag_y_sum / sfr::imu::mag_y_buffer.size());
    // }

    // if (check_repeated_values(sfr::imu::mag_z_buffer)) {
    //     sfr::imu::mag_z_average->set_invalid();
    // } else {
    //     sfr::imu::mag_z_average->set_valid();
    //     sfr::imu::mag_z_average->set_value(Epa_smooth(sfr::imu::mag_z_buffer));
    //         // float mag_z_sum = std::accumulate(sfr::imu::mag_z_buffer.begin(), sfr::imu::mag_z_buffer.end(), 0.0);
    //         // sfr::imu::mag_z_average->set_value(mag_z_sum / sfr::imu::mag_z_buffer.size());
    // }

    if (check_repeated_values(sfr::imu::gyro_x_buffer))
    {
        sfr::imu::gyro_x_average->set_invalid();
    }
    else
    {
        sfr::imu::gyro_x_average->set_valid();
        float gyro_x_sum = std::accumulate(sfr::imu::gyro_x_buffer.begin(), sfr::imu::gyro_x_buffer.end(), 0.0);
        sfr::imu::gyro_x_average->set_value(gyro_x_sum / sfr::imu::gyro_x_buffer.size());
    }

    if (check_repeated_values(sfr::imu::gyro_y_buffer)) {
        sfr::imu::gyro_y_average->set_invalid();
    } else {
        sfr::imu::gyro_y_average->set_valid();
        float gyro_y_sum = std::accumulate(sfr::imu::gyro_y_buffer.begin(), sfr::imu::gyro_y_buffer.end(), 0.0);
        sfr::imu::gyro_y_average->set_value(gyro_y_sum / sfr::imu::gyro_y_buffer.size());
    }

    if (check_repeated_values(sfr::imu::gyro_z_buffer)) {
        sfr::imu::gyro_z_average->set_invalid();
    } else {
        sfr::imu::gyro_z_average->set_valid();
        float gyro_z_sum = std::accumulate(sfr::imu::gyro_z_buffer.begin(), sfr::imu::gyro_z_buffer.end(), 0.0);
        sfr::imu::gyro_z_average->set_value(gyro_z_sum / sfr::imu::gyro_z_buffer.size());
    }
}

void IMUMonitor::transition_to_normal()
{
    // updates imu mode to normal
    // faults are cleared
    // all check flags are set to true
    sfr::imu::mode = sensor_mode_type::normal;
    sfr::imu::mag_x_average->set_valid();
    sfr::imu::mag_y_average->set_valid();
    sfr::imu::mag_z_average->set_valid();
    sfr::imu::gyro_x_average->set_valid();
    sfr::imu::gyro_y_average->set_valid();
    sfr::imu::gyro_z_average->set_valid();
    sfr::imu::acc_x_average->set_valid();
    sfr::imu::acc_y_average->set_valid();
}

void IMUMonitor::transition_to_abnormal_init()
{
    // updates imu mode to abnormal_init
    // trips fault
    // all check flags are set to false
    sfr::imu::mode = sensor_mode_type::abnormal_init;
    sfr::imu::mag_x_average->set_invalid();
    sfr::imu::mag_y_average->set_invalid();
    sfr::imu::mag_z_average->set_invalid();
    sfr::imu::gyro_x_average->set_invalid();
    sfr::imu::gyro_y_average->set_invalid();
    sfr::imu::gyro_z_average->set_invalid();
    sfr::imu::acc_x_average->set_invalid();
    sfr::imu::acc_y_average->set_invalid();
}

void IMUMonitor::transition_to_retry()
{
    // updates imu mode to retry
    // this mode will call either transition_to_normal or transition_to_abnormal_init, which will flip flags
    // called when command to retry processes
    sfr::imu::mode = sensor_mode_type::retry;
}
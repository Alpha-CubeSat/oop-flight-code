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

    int first_val = buffer[0];
    for (float val : buffer) {
        if (first_val != val) {
            return false;
        }
    }

    return true;
}

void IMUMonitor::capture_imu_values()
{
    sensors_event_t accel, mag, gyro, temp;
    imu.getEvent(&accel, &mag, &gyro, &temp);

    imu.setupMag(imu.LSM9DS1_MAGGAIN_8GAUSS);
    imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);

    // Save most recent readings

    sfr::imu::mag_x = mag.magnetic.x;
    sfr::imu::mag_y = mag.magnetic.y;
    sfr::imu::mag_z = mag.magnetic.z;

    sfr::imu::gyro_x = gyro.gyro.x;
    sfr::imu::gyro_y = gyro.gyro.y;
    sfr::imu::gyro_z = gyro.gyro.z;

    sfr::imu::gyro_x_value->set_value(sfr::imu::gyro_x);
    sfr::imu::gyro_y_value->set_value(sfr::imu::gyro_y);
    sfr::imu::gyro_z_value->set_value(sfr::imu::gyro_z);

    // Add reading to buffer

    sfr::imu::mag_x_buffer.push_front(mag.magnetic.x);
    sfr::imu::mag_y_buffer.push_front(mag.magnetic.y);
    sfr::imu::mag_z_buffer.push_front(mag.magnetic.z);

    sfr::imu::gyro_x_buffer.push_front(gyro.gyro.x);
    sfr::imu::gyro_y_buffer.push_front(gyro.gyro.y);
    sfr::imu::gyro_z_buffer.push_front(gyro.gyro.z);

    // Remove old readings

    if (sfr::imu::mag_x_buffer.size() > constants::sensor::collect) {
        sfr::imu::mag_x_buffer.pop_back();
    }
    if (sfr::imu::mag_y_buffer.size() > constants::sensor::collect) {
        sfr::imu::mag_y_buffer.pop_back();
    }
    if (sfr::imu::mag_z_buffer.size() > constants::sensor::collect) {
        sfr::imu::mag_z_buffer.pop_back();
    }
    if (sfr::imu::gyro_x_buffer.size() > constants::sensor::collect) {
        sfr::imu::gyro_x_buffer.pop_back();
    }
    if (sfr::imu::gyro_y_buffer.size() > constants::sensor::collect) {
        sfr::imu::gyro_y_buffer.pop_back();
    }
    if (sfr::imu::gyro_z_buffer.size() > constants::sensor::collect) {
        sfr::imu::gyro_z_buffer.pop_back();
    }

    // Check for repeated/invalid readings, calculate sums and average

    if (check_repeated_values(sfr::imu::mag_x_buffer)) {
        sfr::imu::mag_x_average->set_invalid();
    } else {
        sfr::imu::mag_x_average->set_valid();
        float mag_x_sum = std::accumulate(sfr::imu::mag_x_buffer.begin(), sfr::imu::mag_x_buffer.end(), 0.0);
        sfr::imu::mag_x_average->set_value(mag_x_sum / sfr::imu::mag_x_buffer.size());
    }

    if (check_repeated_values(sfr::imu::mag_y_buffer)) {
        sfr::imu::mag_y_average->set_invalid();
    } else {
        sfr::imu::mag_y_average->set_valid();
        float mag_y_sum = std::accumulate(sfr::imu::mag_y_buffer.begin(), sfr::imu::mag_y_buffer.end(), 0.0);
        sfr::imu::mag_y_average->set_value(mag_y_sum / sfr::imu::mag_y_buffer.size());
    }

    if (check_repeated_values(sfr::imu::mag_z_buffer)) {
        sfr::imu::mag_z_average->set_invalid();
    } else {
        sfr::imu::mag_z_average->set_valid();
        float mag_z_sum = std::accumulate(sfr::imu::mag_z_buffer.begin(), sfr::imu::mag_z_buffer.end(), 0.0);
        sfr::imu::mag_z_average->set_value(mag_z_sum / sfr::imu::mag_z_buffer.size());
    }

    if (check_repeated_values(sfr::imu::gyro_x_buffer)) {
        sfr::imu::gyro_x_average->set_invalid();
    } else {
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
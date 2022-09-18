#include "IMUMonitor.hpp"

// eliminate virtual classes and camera monitor.cpp
// adapt to new initialization process
// if in case 2 for two minutes, it has failed initialization

IMUMonitor::IMUMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    imu = Adafruit_LSM9DS1(constants::imu::CSAG, constants::imu::CSM);

    IMUMonitor::IMU_setup();
}

void IMUMonitor::IMU_setup()
{
    if (!imu.begin()) {
        Serial.println("\n\n\nInitialize failed\n\n\n");
        sfr::imu::successful_init = false;
    } else {
        Serial.println("\n\n\nInitialize successfully\n\n\n");
        sfr::imu::successful_init = true;
        imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
        imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
        imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
    }
}

void IMUMonitor::execute()
{
    if (sfr::imu::turn_off == true && sfr::imu::powered == true) {
#ifdef VERBOSE
        Serial.println("turned off IMU");
#endif
        pinMode(constants::imu::CSAG, OUTPUT);
        pinMode(constants::imu::CSM, OUTPUT);
        Pins::setPinState(constants::imu::CSAG, LOW);
        Pins::setPinState(constants::imu::CSM, LOW);

        sfr::imu::powered = false;
        sfr::imu::turn_off = false;
    }

    if (sfr::imu::turn_on == true && sfr::imu::powered == false) {
#ifdef VERBOSE
        Serial.println("turned on IMU");
#endif
        sfr::imu::turn_on = false; // we don't want to initialize every time

        if (sfr::imu::successful_init) {
            sfr::imu::powered = true;
            transition_to_normal();
        } else {
            transition_to_abnormal_init();
            IMUMonitor::IMU_setup();
        }
    }

    if (sfr::imu::powered == true) {
#ifdef VERBOSE
        Serial.println("IMU is on");
#endif
        capture_imu_values();
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

    // for testing
    Serial.printf("\n\nx : %f, y: %f, z: %f\n\n", sfr::imu::gyro_x, sfr::imu::gyro_y, sfr::imu::gyro_z);

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
    sfr::imu::mag_x_average->set_invalid();
    sfr::imu::mag_y_average->set_invalid();
    sfr::imu::mag_z_average->set_invalid();
    sfr::imu::gyro_x_average->set_invalid();
    sfr::imu::gyro_y_average->set_invalid();
    sfr::imu::gyro_z_average->set_invalid();
    sfr::imu::acc_x_average->set_invalid();
    sfr::imu::acc_y_average->set_invalid();
}

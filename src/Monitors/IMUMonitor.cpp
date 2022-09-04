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
    mode = sensor_mode_type::init; // This is already done in sfr.cpp but included for clarity
}

void IMUMonitor::execute()
{
    if (sfr::imu::sample) {
        switch (mode) {
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

void IMUMonitor::capture_imu_values()
{
    sensors_event_t accel, mag, gyro, temp;
    imu.getEvent(&accel, &mag, &gyro, &temp);

    imu.setupMag(imu.LSM9DS1_MAGGAIN_8GAUSS);
    imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);

    // Save most recent readings

    sfr::imu::mag_x_value->set_value(mag.magnetic.x);
    sfr::imu::mag_y_value->set_value(mag.magnetic.y);
    sfr::imu::mag_z_value->set_value(mag.magnetic.z);

    sfr::imu::gyro_x_value->set_value(gyro.gyro.x);
    sfr::imu::gyro_y_value->set_value(gyro.gyro.y);
    sfr::imu::gyro_z_value->set_value(gyro.gyro.z);

    // Add reading to buffer

    sfr::imu::mag_x_average->set_value(mag.magnetic.x);
    sfr::imu::mag_y_average->set_value(mag.magnetic.y);
    sfr::imu::mag_z_average->set_value(mag.magnetic.z);

    sfr::imu::gyro_x_average->set_value(gyro.gyro.x);
    sfr::imu::gyro_y_average->set_value(gyro.gyro.y);
    sfr::imu::gyro_z_average->set_value(gyro.gyro.z);
}

void IMUMonitor::transition_to_normal()
{
    // updates imu mode to normal
    // faults are cleared
    // all check flags are set to true
    mode = sensor_mode_type::normal;
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
    mode = sensor_mode_type::abnormal_init;
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
    sfr::imu::mode = (uint16_t)sensor_mode_type::retry;
}
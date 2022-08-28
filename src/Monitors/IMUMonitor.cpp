#include "IMUMonitor.hpp"

// eliminate virtual classes and camera monitor.cpp
// adapt to new initialization process
// if in case 2 for two minutes, it has failed initialization

IMUMonitor::IMUMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    gyro_x_value = new SensorReading(1, 0, 0);
    gyro_y_value = new SensorReading(1, 0, 0);
    gyro_z_value = new SensorReading(1, 0, 0);

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

    mag_x_value->set_value(mag.magnetic.x);
    mag_y_value->set_value(mag.magnetic.y);
    mag_z_value->set_value(mag.magnetic.z);

    gyro_x_value->set_value(gyro.gyro.x);
    gyro_y_value->set_value(gyro.gyro.y);
    gyro_z_value->set_value(gyro.gyro.z);

    // Add reading to buffer

    mag_x_average->set_value(mag.magnetic.x);
    mag_y_average->set_value(mag.magnetic.y);
    mag_z_average->set_value(mag.magnetic.z);

    gyro_x_average->set_value(gyro.gyro.x);
    gyro_y_average->set_value(gyro.gyro.y);
    gyro_z_average->set_value(gyro.gyro.z);
}

void IMUMonitor::transition_to_normal()
{
    // updates imu mode to normal
    // faults are cleared
    // all check flags are set to true
    mode = sensor_mode_type::normal;
    mag_x_average->set_valid();
    mag_y_average->set_valid();
    mag_z_average->set_valid();
    gyro_x_average->set_valid();
    gyro_y_average->set_valid();
    gyro_z_average->set_valid();
    acc_x_average->set_valid();
    acc_y_average->set_valid();
}

void IMUMonitor::transition_to_abnormal_init()
{
    // updates imu mode to abnormal_init
    // trips fault
    // all check flags are set to false
    mode = sensor_mode_type::abnormal_init;
    mag_x_average->set_invalid();
    mag_y_average->set_invalid();
    mag_z_average->set_invalid();
    gyro_x_average->set_invalid();
    gyro_y_average->set_invalid();
    gyro_z_average->set_invalid();
    acc_x_average->set_invalid();
    acc_y_average->set_invalid();
}

void IMUMonitor::transition_to_retry()
{
    // updates imu mode to retry
    // this mode will call either transition_to_normal or transition_to_abnormal_init, which will flip flags
    // called when command to retry processes
    sfr::imu::mode = sensor_mode_type::retry;
}
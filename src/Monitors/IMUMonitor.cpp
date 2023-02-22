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

    sfr::imu::mag_x_value->set_value(mag.magnetic.x);
    sfr::imu::mag_y_value->set_value(mag.magnetic.y);
    sfr::imu::mag_z_value->set_value(mag.magnetic.z);

    sfr::imu::gyro_x_value->set_value(gyro.gyro.x);
    sfr::imu::gyro_y_value->set_value(gyro.gyro.y);
    sfr::imu::gyro_z_value->set_value(gyro.gyro.z);
    sfr::imu::collection_time = millis();
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
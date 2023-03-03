#include "IMUMonitor.hpp"

// eliminate virtual classes and camera monitor.cpp
// adapt to new initialization process
// if in case 2 for two minutes, it has failed initialization

IMUMonitor::IMUMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    imu = Adafruit_LSM9DS1(constants::imu::CSAG, constants::imu::CSM);
}

void IMUMonitor::IMU_setup()
{
    if (!imu.begin()) {
        Serial.println("\n\n\nInitialize failed\n\n\n");
        sfr::imu::successful_init = false;
        transition_to_normal();
    } else {
        Serial.println("\n\n\nInitialize successful\n\n\n");
        sfr::imu::successful_init = true;
        transition_to_abnormal_init();
        imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
        imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
        imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
    }
}

void IMUMonitor::execute()
{
    if (sfr::imu::mode == (uint16_t)sensor_mode_type::init) {
        /* IMPORTANT NOTE:
        The IMUMonitor::IMU_setup() cannot be called in the constructor! This is due to a constructor
        trying to call a class method when the object is still being created. If the code inside
        IMUMonitor::IMU_setup() is placed inside the constructor, certain SFR values will not update
        because the header file is not yet loaded in, and the transition_to_normal and transition_to_abnormal
        functions would have to be moved to outside the constructor into the execute function.
        Previously, when IMUMonitor::IMU_setup() was called in the constructor, the IMU would not get
        initialized immediately, but further along in the IMUMonitor::execute() when caught in the

        if (sfr::imu::successful_init) {...}

        block on line 67.
        */
        IMU_setup();
    }

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
        } else if (sfr::detumble::num_imu_retries < sfr::detumble::max_imu_retries) {
            IMUMonitor::IMU_setup();
            sfr::detumble::num_imu_retries++;
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

    // IMU PRINT STATEMENTS FOR LOGGING AND GRAPHING IMU DATA
    Serial.print("Gyro_X: ");
    Serial.print(gyro.gyro.x);
    Serial.print(" Gyro_Y: ");
    Serial.print(gyro.gyro.y);
    Serial.print(" Gyro_Z: ");
    Serial.print(gyro.gyro.z);
    Serial.print(" Time: ");
    Serial.println(millis());

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
    sfr::imu::mode = (uint16_t)sensor_mode_type::normal;
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
    sfr::imu::mode = (uint16_t)sensor_mode_type::abnormal_init;
    sfr::imu::mag_x_average->set_invalid();
    sfr::imu::mag_y_average->set_invalid();
    sfr::imu::mag_z_average->set_invalid();
    sfr::imu::gyro_x_average->set_invalid();
    sfr::imu::gyro_y_average->set_invalid();
    sfr::imu::gyro_z_average->set_invalid();
    sfr::imu::acc_x_average->set_invalid();
    sfr::imu::acc_y_average->set_invalid();
}
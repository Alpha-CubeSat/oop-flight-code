#include "IMUMonitor.hpp"

// eliminate virtual classes and camera monitor.cpp
// adapt to new initialization process
// if in case 2 for two minutes, it has failed initialization

IMUMonitor::IMUMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
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
            // get the start time for the collection of data
            sfr::imu::imu_boot_collection_start_time = millis();
            sfr::imu::sample_gyro = true;
        }
    }
}

void IMUMonitor::execute()
{
    // handle latent turn on / turn off variables
    if (sfr::imu::turn_off == true && sfr::imu::powered == false) {
        sfr::imu::turn_off = false;
    }
    if (sfr::imu::turn_on == true && sfr::imu::powered == true) {
        sfr::imu::turn_on = false;
    }

    if (sfr::imu::turn_on == true && sfr::imu::powered == false) {
#ifdef VERBOSE
        Serial.println("Turned on IMU");
#endif
        IMUMonitor::IMU_init();
        if (sfr::imu::init_mode == (uint16_t)sensor_init_mode_type::complete) {
            sfr::imu::turn_on = false;
            transition_to_normal();
            sfr::imu::powered = true;
        } else {
            if (sfr::imu::failed_times == sfr::imu::failed_limit) {
                sfr::imu::failed_times = 0; // reset
                transition_to_abnormal_init();
            } else {
                sfr::imu::failed_times = sfr::imu::failed_times + 1;
#ifdef VERBOSE
                Serial.print("IMU initialization failed times: ");
                Serial.println(sfr::imu::failed_times);
#endif
                sfr::imu::init_mode = (uint16_t)sensor_init_mode_type::awaiting;
            }
        }
    }

    if (sfr::imu::turn_off == true && sfr::imu::powered == true) {
#ifdef VERBOSE
        Serial.println("Turned off IMU");
#endif
        pinMode(constants::imu::CSAG, OUTPUT);
        pinMode(constants::imu::CSM, OUTPUT);
        Pins::setPinState(constants::imu::CSAG, LOW);
        Pins::setPinState(constants::imu::CSM, LOW);

        sfr::imu::powered = false;
        sfr::imu::turn_off = false;

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
#ifdef IMU_TESTING
    Serial.print("Gyro_X: ");
    Serial.print(gyro.gyro.x);
    Serial.print(" Gyro_Y: ");
    Serial.print(gyro.gyro.y);
    Serial.print(" Gyro_Z: ");
    Serial.print(gyro.gyro.z);
    Serial.print(" Time: ");
    Serial.println(millis());
#endif

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

    sfr::imu::mag_x_value->set_valid();
    sfr::imu::mag_y_value->set_valid();
    sfr::imu::mag_z_value->set_valid();
    sfr::imu::gyro_x_value->set_valid();
    sfr::imu::gyro_y_value->set_valid();
    sfr::imu::gyro_z_value->set_valid();
}

void IMUMonitor::transition_to_abnormal_init()
{
    // updates imu mode to abnormal_init
    // trips fault
    // all check flags are set to false
    sfr::imu::mode = (uint16_t)sensor_mode_type::abnormal_init;
    invalidate_data();

    sfr::imu::turn_on = false;
    sfr::imu::powered = false;
    sfr::imu::turn_off = false;
    sfr::imu::init_mode = (uint16_t)sensor_init_mode_type::awaiting;
}

void IMUMonitor::invalidate_data()
{
    sfr::imu::mag_x_average->set_invalid();
    sfr::imu::mag_y_average->set_invalid();
    sfr::imu::mag_z_average->set_invalid();
    sfr::imu::gyro_x_average->set_invalid();
    sfr::imu::gyro_y_average->set_invalid();
    sfr::imu::gyro_z_average->set_invalid();

    sfr::imu::mag_x_value->set_invalid();
    sfr::imu::mag_y_value->set_invalid();
    sfr::imu::mag_z_value->set_invalid();
    sfr::imu::gyro_x_value->set_invalid();
    sfr::imu::gyro_y_value->set_invalid();
    sfr::imu::gyro_z_value->set_invalid();
}
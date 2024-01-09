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

#ifndef ACS_SIM
    // used outside of ACS Control Task to determine exit conditions for Detumble Spin
    sfr::imu::gyro_x_average->set_value(gyro.gyro.x);
    sfr::imu::gyro_y_average->set_value(gyro.gyro.y);
    sfr::imu::gyro_z_average->set_value(gyro.gyro.z);
#endif
}

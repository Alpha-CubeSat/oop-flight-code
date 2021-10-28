#include "IMUMonitor.hpp"

// make sfr values?
uint32_t start;
uint32_t stop;
bool began;

IMUMonitor::IMUMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    imu = Adafruit_LSM9DS1(constants::imu::CSAG, constants::imu::CSM);
    start = millis();
    if (!imu.begin())
    {
        stop = millis();
        began = 0;
        transition_to_abnormal();
        //sfr::rockblock::fault_report[constants::fault::imu_begin] = 1;
    } else {
        stop = millis();
        began = 1;
    }
    imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
    imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
    imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
}

void IMUMonitor::execute()
{
    if(check_sensor == false){
        mode = sensor_mode_type::abandon;
    }
    if(check_sensor == true && mode == sensor_mode_type::abandon){
        if(!imu.begin()){
            delay(1000);
        } else {
            began = 1;
            transition_to_normal();
        }
    }

    switch(mode) {
        case sensor_mode_type::normal:
            Serial.println("IMU is normal");

            sensors_event_t accel, mag, gyro, temp;
            imu.getEvent(&accel, &mag, &gyro, &temp);

            imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
            if (mag.magnetic.x > 4 || mag.magnetic.y > 4 || mag.magnetic.z > 4)
            {
                imu.setupMag(imu.LSM9DS1_MAGGAIN_8GAUSS);
            }
            if (mag.magnetic.x > 8 || mag.magnetic.y > 8 || mag.magnetic.z > 8)
            {
                imu.setupMag(imu.LSM9DS1_MAGGAIN_12GAUSS);
            }
            if (mag.magnetic.x > 12 || mag.magnetic.y > 12 || mag.magnetic.z > 12)
            {
                imu.setupMag(imu.LSM9DS1_MAGGAIN_16GAUSS);
            }

            imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
            if (gyro.gyro.x > 245 || gyro.gyro.y > 245 || gyro.gyro.z > 245)
            {
                imu.setupGyro(imu.LSM9DS1_GYROSCALE_500DPS);
            }
            if (gyro.gyro.x > 500 || gyro.gyro.y > 500 || gyro.gyro.z > 500)
            {
                imu.setupGyro(imu.LSM9DS1_GYROSCALE_2000DPS);
            }

            // Get corrected IMU values

            imu.getEvent(&accel, &mag, &gyro, &temp);

            // Save most recent readings

            sfr::imu::mag_x = mag.magnetic.x;
            sfr::imu::mag_y = mag.magnetic.y;
            sfr::imu::mag_z = mag.magnetic.z;

            sfr::imu::gyro_x = gyro.gyro.x;
            sfr::imu::gyro_y = gyro.gyro.y;
            sfr::imu::gyro_z = gyro.gyro.z;

            // Add reading to buffer

            sfr::imu::mag_x_buffer.push_front(mag.magnetic.x);
            sfr::imu::mag_y_buffer.push_front(mag.magnetic.y);
            sfr::imu::mag_z_buffer.push_front(mag.magnetic.z);

            sfr::imu::gyro_x_buffer.push_front(gyro.gyro.x);
            sfr::imu::gyro_y_buffer.push_front(gyro.gyro.y);
            sfr::imu::gyro_z_buffer.push_front(gyro.gyro.z);

            // Remove old readings

            if (sfr::imu::mag_x_buffer.size() > constants::sensor::collect)
            {
                sfr::imu::mag_x_buffer.pop_back();
            }
            if (sfr::imu::mag_y_buffer.size() > constants::sensor::collect)
            {
                sfr::imu::mag_y_buffer.pop_back();
            }
            if (sfr::imu::mag_z_buffer.size() > constants::sensor::collect)
            {
                sfr::imu::mag_z_buffer.pop_back();
            }
            if (sfr::imu::gyro_x_buffer.size() > constants::sensor::collect)
            {
                sfr::imu::gyro_x_buffer.pop_back();
            }
            if (sfr::imu::gyro_y_buffer.size() > constants::sensor::collect)
            {   
                sfr::imu::gyro_y_buffer.pop_back();
            }
            if (sfr::imu::gyro_z_buffer.size() > constants::sensor::collect)
            {
                sfr::imu::gyro_z_buffer.pop_back();
            }

            // Calculate sums

            float mag_x_sum = std::accumulate(sfr::imu::mag_x_buffer.begin(), sfr::imu::mag_x_buffer.end(), 0.0);
            float mag_y_sum = std::accumulate(sfr::imu::mag_y_buffer.begin(), sfr::imu::mag_y_buffer.end(), 0.0);
            float mag_z_sum = std::accumulate(sfr::imu::mag_z_buffer.begin(), sfr::imu::mag_z_buffer.end(), 0.0);
            float gyro_x_sum = std::accumulate(sfr::imu::gyro_x_buffer.begin(), sfr::imu::gyro_x_buffer.end(), 0.0);
            float gyro_y_sum = std::accumulate(sfr::imu::gyro_y_buffer.begin(), sfr::imu::gyro_y_buffer.end(), 0.0);
            float gyro_z_sum = std::accumulate(sfr::imu::gyro_z_buffer.begin(), sfr::imu::gyro_z_buffer.end(), 0.0);

            // Calculate averages

            sfr::imu::mag_x_average = mag_x_sum / sfr::imu::mag_x_buffer.size();
            sfr::imu::mag_y_average = mag_y_sum / sfr::imu::mag_y_buffer.size();
            sfr::imu::mag_z_average = mag_z_sum / sfr::imu::mag_z_buffer.size();

            sfr::imu::gyro_x_average = gyro_x_sum / sfr::imu::gyro_x_buffer.size();
            sfr::imu::gyro_y_average = gyro_y_sum / sfr::imu::gyro_y_buffer.size();
            sfr::imu::gyro_z_average = gyro_z_sum / sfr::imu::gyro_z_buffer.size();
            break;
        case sensor_mode_type::abnormal:
            Serial.println("imu is abnormal");
            Serial.println("trying again...");
            if(!imu.begin()){
                delay(1000);
                break;
            } else {
                began = 1;
                transition_to_normal();
            }
            break;
        case sensor_mode_type::abandon:
            transition_to_abandon();
            break;
    }
}

void IMUMonitor::transition_to_normal() {
    // updates imu mode to normal
    // all check flags are reset to true
    mode = sensor_mode_type::normal;
    // sfr::fault::fault1 = 0;
    sfr::fault::check_mag_x = true;
    sfr::fault::check_mag_y = true;
    sfr::fault::check_mag_z = true;
    sfr::fault::check_gyro_x = true;
    sfr::fault::check_gyro_y = true;
    sfr::fault::check_gyro_z = true;
    sfr::fault::check_acc_x = true;
    sfr::fault::check_acc_y = true;
}

void IMUMonitor::transition_to_abnormal() {
    // updates imu mode to abnormal
    // triggers transition to safe mode by tripping fault
    // all check flags are false
    Serial.print("IMU init: ");
    began ? Serial.println("Success") : Serial.println("Failed");
    Serial.print("Time taken: "); Serial.println(stop-start);

    mode = sensor_mode_type::abnormal;
    if(!began){
        sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::init;
    }
    if (sfr::imu::gyro_y_buffer.size() > constants::sensor::collect)
    {
        sfr::imu::gyro_y_buffer.pop_back();
    }
    if (sfr::imu::gyro_z_buffer.size() > constants::sensor::collect)
    {
        sfr::imu::gyro_z_buffer.pop_back();
    }

    // Calculate sums

    float mag_x_sum = std::accumulate(sfr::imu::mag_x_buffer.begin(), sfr::imu::mag_x_buffer.end(), 0.0);
    float mag_y_sum = std::accumulate(sfr::imu::mag_y_buffer.begin(), sfr::imu::mag_y_buffer.end(), 0.0);
    float mag_z_sum = std::accumulate(sfr::imu::mag_z_buffer.begin(), sfr::imu::mag_z_buffer.end(), 0.0);
    float gyro_x_sum = std::accumulate(sfr::imu::gyro_x_buffer.begin(), sfr::imu::gyro_x_buffer.end(), 0.0);
    float gyro_y_sum = std::accumulate(sfr::imu::gyro_y_buffer.begin(), sfr::imu::gyro_y_buffer.end(), 0.0);
    float gyro_z_sum = std::accumulate(sfr::imu::gyro_z_buffer.begin(), sfr::imu::gyro_z_buffer.end(), 0.0);

    // Calculate averages

    sfr::imu::mag_x_average = mag_x_sum / sfr::imu::mag_x_buffer.size();
    sfr::imu::mag_y_average = mag_y_sum / sfr::imu::mag_y_buffer.size();
    sfr::imu::mag_z_average = mag_z_sum / sfr::imu::mag_z_buffer.size();

    sfr::imu::gyro_x_average = gyro_x_sum / sfr::imu::gyro_x_buffer.size();
    sfr::imu::gyro_y_average = gyro_y_sum / sfr::imu::gyro_y_buffer.size();
    sfr::imu::gyro_z_average = gyro_z_sum / sfr::imu::gyro_z_buffer.size();

    #ifdef VERBOSE
    Serial.print("Mag: "); Serial.print(sfr::imu::mag_x_average); Serial.print(",");
    Serial.print(sfr::imu::mag_y_average); Serial.print(",");
    Serial.print(sfr::imu::mag_z_average); Serial.println(" Gauss");
    Serial.print("Gyro: "); Serial.print(sfr::imu::gyro_x_average); Serial.print(",");
    Serial.print(sfr::imu::gyro_y_average); Serial.print(",");
    Serial.print(sfr::imu::gyro_z_average); Serial.println(" deg/s");
    #endif
    uint32_t end = micros();
}

void IMUMonitor::transition_to_abandon() {
    // all check flags are false (ordered to abandon sensor)
    sfr::fault::check_mag_x = false;
    sfr::fault::check_mag_y = false;
    sfr::fault::check_mag_z = false;
    sfr::fault::check_gyro_x = false;
    sfr::fault::check_gyro_y = false;
    sfr::fault::check_gyro_z = false;
    sfr::fault::check_acc_x = false;
    sfr::fault::check_acc_y = false;
}
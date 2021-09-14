#include "IMUMonitor.hpp"

IMUMonitor::IMUMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    imu = Adafruit_LSM9DS1(constants::imu::CSAG, constants::imu::CSM);
    imu.begin();
    if (!imu.begin())
    {
        //sfr::rockblock::fault_report[constants::fault::imu_begin] = 1;
    }
    imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
    imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
    imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
}

void IMUMonitor::execute()
{
    uint32_t begin = micros();
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
    uint32_t end = micros();
    Serial.print("IMU_MON:");
    Serial.println(end - begin);
}

#include "IMUDownlink.hpp"

IMUDownlink::IMUDownlink(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    imu = Adafruit_LSM9DS1(constants::imu::CSAG, constants::imu::CSM);
    imu.begin();
    //if(!imu.begin()) then...
    imu.readGyro();
    imu.readMag();
}

void IMUDownlink::execute()
{
    if (sfr::mission::mode == mission_mode_type::mand_burns || sfr::mission::mode == mission_mode_type::reg_burns) {
        uint32_t begin = micros();
        sensors_event_t accel, mag, gyro, temp;
        imu.getEvent(&accel, &mag, &gyro, &temp);

        imu.readMag();

        if (mag.magnetic.x > 4 || mag.magnetic.y > 4 || mag.magnetic.z > 4) {
            sfr::imu::imu_dlink_magid_type = imu_downlink_type::GAUSS_8;
        }
        if (mag.magnetic.x > 8 || mag.magnetic.y > 8 || mag.magnetic.z > 8) {
            imu_downlink_type::GAUSS_8;
            sfr::imu::imu_dlink_magid_type = imu_downlink_type::GAUSS_12;
        }
        if (mag.magnetic.x > 12 || mag.magnetic.y > 12 || mag.magnetic.z > 12) {
            sfr::imu::imu_dlink_magid_type = imu_downlink_type::GAUSS_16;
        }

        // Get corrected IMU values
        imu.readGyro();

        imu.getEvent(&accel, &mag, &gyro, &temp);

        // !!!!!!if(imu sample == true) { add values to buffer}
        // boolean trans_to_photo = false;
        // // need to be replaced by another conditional value
        // Serial.printf("%s\n", (MissionManager.transition_to_photo(), "transition_to_photo() was called."));
        // // mission mode transitions to take photo
        // trans_to_photo = true;

        // Add reading to imu downlink buffer

        sfr::imu::imu_dlink_magid_buffer.push_front(sfr::imu::imu_dlink_magid_type);
        sfr::imu::gyro_x_buffer.push_front(gyro.gyro.x);
        sfr::imu::gyro_y_buffer.push_front(gyro.gyro.y);
        sfr::imu::gyro_z_buffer.push_front(gyro.gyro.z);
        sfr::imu::imu_dlink_time_buffer.push_front(millis());

        // Remove old readings

        if (sfr::imu::imu_dlink_magid_buffer.size() > constants::sensor::collect) {
            sfr::imu::imu_dlink_magid_buffer.pop_back();
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
        if (sfr::imu::imu_dlink_time_buffer.size() > constants::sensor::collect) {
            sfr::imu::imu_dlink_time_buffer.pop_back();
        }

        uint32_t end = micros();
    }
}
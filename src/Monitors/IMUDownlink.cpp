#include "IMUDownlink.hpp"

IMUDownlink::IMUDownlink(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void IMUDownlink::execute()
{
    float gyro_x;
    float gyro_y;
    float gyro_z;

    if (sfr::imu::sample_gyro) {

        // Add reading to imu downlink buffer

        sfr::imu::gyro_x_value->get_value(&gyro_x);
        sfr::imu::gyro_y_value->get_value(&gyro_y);
        sfr::imu::gyro_z_value->get_value(&gyro_z);

        // PRINT STATMENTS FOR SENDING OUT IMU DATA
        Serial.print("Gyro_X: ");
        Serial.print((gyro_x));
        Serial.print(" Gyro_Y: ");
        Serial.print((gyro_y));
        Serial.print(" Gyro_Z: ");
        Serial.print((gyro_z));
        Serial.print(" Time: ");
        Serial.println(sfr::imu::collection_time);

        sfr::imu::imu_dlink.push_front((gyro_x + 5) * 25);
        sfr::imu::imu_dlink.push_front((gyro_y + 5) * 25);
        sfr::imu::imu_dlink.push_front((gyro_z + 5) * 25);
    }

    if (sfr::mission::current_mode->get_id() == sfr::mission::mandatoryBurns->get_id() || sfr::mission::current_mode->get_id() == sfr::mission::regularBurns->get_id()) {
        sfr::imu::sample_gyro = true;
    }

    // need to be stored in sfr later (time to record imu data after deployment)
    // 60 seconds is about 14 packets
    if ((sfr::mission::current_mode == sfr::mission::photo || sfr::mission::already_deployed) && millis() - sfr::mission::time_deployed > 10 * constants::time::one_second) {
        sfr::imu::sample_gyro = false;
        sfr::imu::turn_off = true;
        sfr::imu::report_written = true;
    } else if (sfr::mission::current_mode == sfr::mission::photo) {
        if (!sfr::mission::already_deployed) {
            sfr::mission::time_deployed = millis();
            sfr::mission::already_deployed = true;
        }
        sfr::imu::sample_gyro = true;
    }
}
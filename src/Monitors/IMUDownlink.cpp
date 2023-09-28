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
    static bool prev_pressed = true;
    if (sfr::imu::sample_gyro) {
        // collect the time where door opened
        if (prev_pressed != sfr::button::pressed) {
            sfr::imu::door_open_collection_start_time = millis();
        }

        // this stops the data from being collected 10 seconds after the door opens.
        if (sfr::button::pressed || (millis() - sfr::imu::door_open_collection_start_time < constants::imu::after_door_opens_min_run_time)) {

            // Add reading to imu downlink buffer
            sfr::imu::gyro_x_value->get_value(&gyro_x);
            sfr::imu::gyro_y_value->get_value(&gyro_y);
            sfr::imu::gyro_z_value->get_value(&gyro_z);

            sfr::imu::imu_dlink.push_front((gyro_x + 5) * 25);
            sfr::imu::imu_dlink.push_front((gyro_y + 5) * 25);
            sfr::imu::imu_dlink.push_front((gyro_z + 5) * 25);

            // NOTE: For the down link we are enforcing a FIFO to keep the amound of data we send down low.
            // We want about 30 seconds of data which comes out to be around 462 bytes so we are enforcing that here.
            if (sfr::imu::imu_dlink.size() > constants::imu_downlink::downlink_FIFO_byte_length) {
                // if greater than the max size shrink the buffer.
                for (int i = 0; i < 3; i++) {
                    sfr::imu::imu_dlink.pop_back();
                }
            }
            prev_pressed = sfr::button::pressed;
        }

        // added to or statment for the IMU booting
        if (sfr::mission::current_mode->get_id() == sfr::mission::bootImu->get_id() || sfr::mission::current_mode->get_id() == sfr::mission::mandatoryBurns->get_id() || sfr::mission::current_mode->get_id() == sfr::mission::regularBurns->get_id()) {
            sfr::imu::sample_gyro = true;
        }

        // need to be stored in sfr later (time to record imu data after deployment)
        // 60 seconds is about 14 packets
        if (sfr::mission::deployed && (millis() - sfr::imu::door_open_collection_start_time) > constants::imu::after_door_opens_min_run_time) {
            sfr::imu::sample_gyro = false;
            sfr::imu::turn_off = true;
            sfr::imu::report_written = true;
        }
    }
}
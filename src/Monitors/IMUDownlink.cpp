#include "IMUDownlink.hpp"

IMUDownlink::IMUDownlink(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    imu = Adafruit_LSM9DS1(constants::imu::CSAG, constants::imu::CSM);
    imu.begin();
    // if(!imu.begin()) then...
}

void IMUDownlink::execute()
{
    // if (sfr::mission::current_mode == sfr::mission::mandatoryBurns || sfr::mission::current_mode == sfr::mission::regularBurns) {

    // if (sfr::mission::deployed == true) {
    //     sfr::rockblock::imu_downlink_on = true;
    // }
    if (sfr::rockblock::imu_first_start == true) {
        sfr::rockblock::imu_first_start = false;
        sfr::rockblock::imudownlink_start_time = millis();
    }

    // #ifdef VERBOSE_IMUD
    //     Serial.println("Start time is: " + String(sfr::rockblock::imudownlink_start_time));
    // #endif

    // #ifdef VERBOSE_IMUD // fujia
    //     // Serial.println("IMU downlink got called");
    //     Serial.println("sfr::rockblock::imu_downlink_on: " + String(sfr::rockblock::imu_downlink_on));
    // #endif

    if (sfr::rockblock::imu_downlink_on == true) {
        // #ifdef VERBOSE // fujia
        //         Serial.println("Timer is on. I'm adding data");
        // #endif

        // #ifdef VERBOSE_IMUD
        //         Serial.println("adding to buffer gyro_x_val: " + String(sfr::imu::gyro_x));
        //         Serial.println("adding to buffer gyro_y_val: " + String(sfr::imu::gyro_y));
        //         Serial.println("adding to buffer gyro_z_val: " + String(sfr::imu::gyro_z));
        // #endif

        sfr::imu::gyro_x = sfr::imu::gyro_x_value->get_value();
        sfr::imu::gyro_y = sfr::imu::gyro_y_value->get_value();
        sfr::imu::gyro_z = sfr::imu::gyro_z_value->get_value();
        // #ifdef VERBOSE_IMUM // fujia
        //         Serial.println("HERE!!!!!!!!!gyro_x: " + String(sfr::imu::gyro_x));
        //         Serial.println("HERE!!!!!!!!!gyro_y: " + String(sfr::imu::gyro_y));
        //         Serial.println("HERE!!!!!!!!!gyro_z: " + String(sfr::imu::gyro_z));
        // #endif

        // Add reading to imu downlink buffer
        sfr::imu::imu_dlink_gyro_x_buffer.push_front(sfr::imu::gyro_x);
        sfr::imu::imu_dlink_gyro_y_buffer.push_front(sfr::imu::gyro_y);
        sfr::imu::imu_dlink_gyro_z_buffer.push_front(sfr::imu::gyro_z);

        // // Remove old readings

        // if (sfr::imu::imu_dlink_gyro_x_buffer.size() > sfr::imu::imu_downlink_buffer_max_size) {
        //     sfr::imu::imu_dlink_gyro_x_buffer.pop_back();
        // }
        // if (sfr::imu::imu_dlink_gyro_y_buffer.size() > sfr::imu::imu_downlink_buffer_max_size) {
        //     sfr::imu::imu_dlink_gyro_y_buffer.pop_back();
        // }
        // if (sfr::imu::imu_dlink_gyro_z_buffer.size() > sfr::imu::imu_downlink_buffer_max_size) {
        //     sfr::imu::imu_dlink_gyro_z_buffer.pop_back();
        // }
        // #ifdef VERBOSE // fujia
        //         // Serial.println(String(millis() - sfr::rockblock::imudownlink_start_time));
        //         Serial.println("Size of buffer x is: " + String(sfr::imu::imu_dlink_gyro_x_buffer.size()));

        // #endif

        if (millis() - sfr::rockblock::imudownlink_start_time > 60 * constants::time::one_second) {
            sfr::rockblock::imu_downlink_on = false;
            sfr::imu::data_downlinked = true;
#ifdef VERBOSE_IMUD
            Serial.println("IMU downlink time up!");
#endif
        }

        // sfr::rockblock::imudownlink_remain_time -= millis() - sfr::rockblock::imudownlink_start_time;
        // #ifdef VERBOSE_IMUM
        //         Serial.println("Time remain is: " + String(sfr::rockblock::imudownlink_remain_time));
        //         Serial.println("one minute is: " + String(constants::time::one_minute));
        //         Serial.println("one sec is: " + String(constants::time::one_second));
        // #endif
        //         if (sfr::rockblock::imudownlink_remain_time <= 0.0) {
        //             sfr::rockblock::imu_downlink_on = false;
    }
}
// #ifdef VERBOSE_IMUD // fujia
//     Serial.println("I'm out!");
// #endif

// #ifdef VERBOSE_IMUM
//     Serial.println("IMU downlink end");
// #endif

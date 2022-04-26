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

    sfr::rockblock::imudownlink_start_time = millis();
    // #ifdef VERBOSE_IMUD
    //     Serial.println("Start time is: " + String(sfr::rockblock::imudownlink_start_time));
    // #endif

#ifdef VERBOSE // fujia
    Serial.println("sfr::rockblock::imu_downlink_on: " + String(sfr::rockblock::imu_downlink_on));
#endif

    while (sfr::rockblock::imu_downlink_on == true) {
#ifdef VERBOSE // fujia
        Serial.println("Timer is on. I'm adding data");
#endif

        // #ifdef VERBOSE_IMUD
        //         Serial.println("adding to buffer gyro_x_val: " + String(sfr::imu::gyro_x));
        //         Serial.println("adding to buffer gyro_y_val: " + String(sfr::imu::gyro_y));
        //         Serial.println("adding to buffer gyro_z_val: " + String(sfr::imu::gyro_z));
        // #endif

        // Add reading to imu downlink buffer
        sfr::imu::imu_dlink_gyro_x_buffer.push_front(sfr::imu::gyro_x);
        sfr::imu::imu_dlink_gyro_y_buffer.push_front(sfr::imu::gyro_y);
        sfr::imu::imu_dlink_gyro_z_buffer.push_front(sfr::imu::gyro_z);

// Remove old readings

// while (sfr::imu::imu_dlink_gyro_x_buffer.size() > sfr::imu::imu_downlink_buffer_max_size) {
//     sfr::imu::imu_dlink_gyro_x_buffer.pop_back();
// }
// while (sfr::imu::imu_dlink_gyro_y_buffer.size() > sfr::imu::imu_downlink_buffer_max_size) {
//     sfr::imu::imu_dlink_gyro_y_buffer.pop_back();
// }
// while (sfr::imu::imu_dlink_gyro_z_buffer.size() > sfr::imu::imu_downlink_buffer_max_size) {
//     sfr::imu::imu_dlink_gyro_z_buffer.pop_back();
// }
#ifdef VERBOSE // fujia
        Serial.println(String(millis() - sfr::rockblock::imudownlink_start_time));

#endif
        if (millis() - sfr::rockblock::imudownlink_start_time > 2 * constants::time::one_second) {
            sfr::rockblock::imu_downlink_on = false;
        }
    }
#ifdef VERBOSE // fujia
    Serial.println("I'm out!");
#endif
}
#include "Monitors/IMUMonitor.hpp"
#include <unity.h>

void test_transition_to_normal_init()
{
    IMUMonitor imu_monitor(0);
    sfr::imu::turn_on = true;
    sfr::imu::powered = false;
    // sfr::imu::failed_times = 0;
    // sfr::imu::failed_limit = 1;
    sfr::imu::init_mode = (uint16_t)sensor_init_mode_type::complete;
    imu_monitor.execute();
    TEST_ASSERT_EQUAL((uint16_t)sensor_mode_type::normal, sfr::imu::mode);
    TEST_ASSERT_EQUAL(true, sfr::imu::mag_x_average->is_valid());
    TEST_ASSERT_EQUAL(true, sfr::imu::mag_y_average->is_valid());
    TEST_ASSERT_EQUAL(true, sfr::imu::mag_z_average->is_valid());
    TEST_ASSERT_EQUAL(true, sfr::imu::gyro_x_average->is_valid());
    TEST_ASSERT_EQUAL(true, sfr::imu::gyro_y_average->is_valid());
    TEST_ASSERT_EQUAL(true, sfr::imu::gyro_z_average->is_valid());
    TEST_ASSERT_EQUAL(true, sfr::imu::acc_x_average->is_valid());
    TEST_ASSERT_EQUAL(true, sfr::imu::acc_y_average->is_valid());
}
void test_transition_to_abnormal_init()
{
    IMUMonitor imu_monitor(0);
    sfr::imu::turn_on = true;
    sfr::imu::powered = false;
    sfr::imu::failed_times = 0;
    sfr::imu::failed_limit = 1;
    // fail the first time
    sfr::imu::init_mode = (uint16_t)sensor_init_mode_type::failed;
    imu_monitor.execute();
    TEST_ASSERT_EQUAL(1, sfr::imu::failed_times);
    TEST_ASSERT_EQUAL((uint16_t)sensor_init_mode_type::awaiting, sfr::imu::init_mode);
    TEST_ASSERT_EQUAL(sfr::imu::failed_times, sfr::imu::failed_limit);
    sfr::imu::init_mode = (uint16_t)sensor_init_mode_type::failed;
    // failed_limit reached, transition_to_abnormal_init
    imu_monitor.execute();
    TEST_ASSERT_EQUAL(0, sfr::imu::failed_times);
    TEST_ASSERT_EQUAL((uint16_t)sensor_mode_type::abnormal_init, sfr::imu::mode);
    TEST_ASSERT_EQUAL(false, sfr::imu::mag_x_average->is_valid());
    TEST_ASSERT_EQUAL(false, sfr::imu::mag_y_average->is_valid());
    TEST_ASSERT_EQUAL(false, sfr::imu::mag_z_average->is_valid());
    TEST_ASSERT_EQUAL(false, sfr::imu::gyro_x_average->is_valid());
    TEST_ASSERT_EQUAL(false, sfr::imu::gyro_y_average->is_valid());
    TEST_ASSERT_EQUAL(false, sfr::imu::gyro_z_average->is_valid());
    TEST_ASSERT_EQUAL(false, sfr::imu::acc_x_average->is_valid());
    TEST_ASSERT_EQUAL(false, sfr::imu::acc_y_average->is_valid());
    TEST_ASSERT_EQUAL(false, sfr::imu::turn_on);
    TEST_ASSERT_EQUAL(false, sfr::imu::powered);
    TEST_ASSERT_EQUAL(false, sfr::imu::turn_off);
    TEST_ASSERT_EQUAL((uint16_t)sensor_init_mode_type::awaiting, sfr::imu::init_mode);
}

void test_turn_off()
{
    IMUMonitor imu_monitor(0);
    sfr::imu::turn_off = true;
    sfr::imu::powered = true;
    imu_monitor.execute();
    TEST_ASSERT_EQUAL(false, sfr::imu::powered);
    TEST_ASSERT_EQUAL(false, sfr::imu::turn_off);
    TEST_ASSERT_EQUAL(OUTPUT, *portModeRegister(constants::imu::CSAG));
    TEST_ASSERT_EQUAL(OUTPUT, *portModeRegister(constants::imu::CSM));
    TEST_ASSERT_EQUAL(LOW, sfr::pins::pinMap[constants::imu::CSAG]);
    TEST_ASSERT_EQUAL(LOW, sfr::pins::pinMap[constants::imu::CSM]);
}

int test_imu_monitor()
{
    UNITY_BEGIN();
    RUN_TEST(test_transition_to_normal_init);
    RUN_TEST(test_transition_to_abnormal_init);
    RUN_TEST(test_turn_off);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_imu_monitor();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    Serial.println("IMU Monitor Started");
    test_imu_monitor();
}

void loop() {}
#endif
#include <unity.h>
#include "Control Tasks/BurnwireControlTask.hpp"
#include "MissionManager.hpp"
void test_valid_initialization(){
    BurnwireControlTask burnwire_control_task(0);
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_out_of_order_commands(){
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;

    sfr::burnwire::fire = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(false, sfr::burnwire::fire);

    sfr::burnwire::fire = true;
    sfr::current::in_sun = false;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::current::in_sun = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);


    for(int i=0; i <constants::burnwire::max_attempts; ++i){
        delay(constants::burnwire::burn_time);
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::delay, sfr::burnwire::mode);

        delay(constants::burnwire::burn_wait);
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);
    }

    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

}

void test_max_attempts(){
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;

    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::current::in_sun = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);

    for(int i=0; i <constants::burnwire::max_attempts; ++i){
        delay(constants::burnwire::burn_time);
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::delay, sfr::burnwire::mode);

        delay(constants::burnwire::burn_wait);
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);
    }

    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_exit_deployment(){
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(false, sfr::burnwire::arm);

    sfr::burnwire::arm = true;
    sfr::mission::mode = mission_mode_type::deployment;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::burnwire::arm = true;
    sfr::mission::mode = mission_mode_type::deployment;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::current::in_sun = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::burnwire::arm = true;
    sfr::mission::mode = mission_mode_type::deployment;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::current::in_sun = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::burnwire::arm = true;
    sfr::mission::mode = mission_mode_type::deployment;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::current::in_sun = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);

    delay(constants::burnwire::burn_time);
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::delay, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_sensor(){
    BurnwireControlTask burnwire_control_task(0);
    MissionManager mission_manager(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;

    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::current::in_sun = true;
    burnwire_control_task.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = true;
    burnwire_control_task.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);
    
    sfr::photoresistor::covered = false;
    burnwire_control_task.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);

    sfr::photoresistor::covered = true;
    sfr::button::pressed = false;
    burnwire_control_task.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);

    sfr::photoresistor::covered = false;
    sfr::button::pressed = false;
    burnwire_control_task.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_camera_max_attempts(){
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;

    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::current::in_sun = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = false;
    for(int i=0; i < constants::burnwire::camera_max_attempts; ++i){
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    }

    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(sfr::burnwire::camera_attempts, 0);

    delay(constants::burnwire::burn_time);
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::delay, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_camera_some_attempts(){
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;

    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::current::in_sun = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = false;
    for(int i=0; i < constants::burnwire::camera_max_attempts/2; ++i){
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    }

    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(sfr::burnwire::camera_attempts, 0);

    delay(constants::burnwire::burn_time);
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::delay, sfr::burnwire::mode);

    delay(constants::burnwire::burn_wait);
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);
}

void test_no_solar_current(){
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::current::in_sun = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
}

void test_armed_timeout(){
     BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    delay(constants::burnwire::armed_time);

    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}


int test_burnwire() {
    UNITY_BEGIN();
    //Timeout is long- change in constants for testing
    //RUN_TEST(test_armed_timeout);
    RUN_TEST(test_valid_initialization);
    RUN_TEST(test_no_solar_current);
    RUN_TEST(test_out_of_order_commands);
    RUN_TEST(test_max_attempts);
    RUN_TEST(test_exit_deployment);
    RUN_TEST(test_sensor);
    RUN_TEST(test_camera_max_attempts);
    RUN_TEST(test_camera_some_attempts);
    return UNITY_END();
}


#ifdef DESKTOP
int main() {
    return test_burnwire();
}
#else
#include <Arduino.h>
void setup() {
    delay(2000);
    Serial.begin(9600);
    test_burnwire();
}

void loop() {}
#endif
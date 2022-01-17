#include "Control Tasks/BurnwireControlTask.hpp"
#include "MissionManager.hpp"
#include "sfr.hpp"
#include <unity.h>

void test_valid_initialization()
{
    BurnwireControlTask burnwire_control_task(0);
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_out_of_order_commands()
{
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
    sfr::fault::check_temp_c = false;
    sfr::current::in_sun = false;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::fault::check_temp_c = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);

    for (int i = 0; i < constants::burnwire::max_attempts; ++i) {
        delay(sfr::burnwire::burn_time);
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::delay, sfr::burnwire::mode);

        delay(constants::burnwire::burn_wait);
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);
    }

    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_max_attempts()
{
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;

    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::fault::check_temp_c = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);

    for (int i = 0; i < constants::burnwire::max_attempts; ++i) {
        delay(sfr::burnwire::burn_time);
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::delay, sfr::burnwire::mode);

        delay(constants::burnwire::burn_wait);
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);
    }

    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_exit_deployment()
{
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
    sfr::temperature::in_sun = true;
    sfr::fault::check_temp_c = true;
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
    sfr::temperature::in_sun = true;
    sfr::fault::check_temp_c = true;
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
    sfr::temperature::in_sun = true;
    sfr::fault::check_temp_c = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);

    delay(sfr::burnwire::burn_time);
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::delay, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_sensor()
{
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

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::fault::check_temp_c = true;
    burnwire_control_task.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::camera::powered = true;
    burnwire_control_task.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::photoresistor::covered = false;
    burnwire_control_task.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::photoresistor::covered = true;
    sfr::button::pressed = false;
    burnwire_control_task.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::photoresistor::covered = false;
    sfr::button::pressed = false;
    burnwire_control_task.execute();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_camera_max_attempts()
{
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;

    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::fault::check_temp_c = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = false;
    for (int i = 0; i < constants::burnwire::camera_max_attempts; ++i) {
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    }

    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(sfr::burnwire::camera_attempts, 0);

    delay(sfr::burnwire::burn_time);
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::delay, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_camera_some_attempts()
{
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;

    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::fault::check_temp_c = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = false;
    for (int i = 0; i < constants::burnwire::camera_max_attempts / 2; ++i) {
        burnwire_control_task.execute();
        TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    }

    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);

    sfr::camera::powered = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(sfr::burnwire::camera_attempts, 0);

    delay(sfr::burnwire::burn_time);
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::delay, sfr::burnwire::mode);

    delay(constants::burnwire::burn_wait);
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::burn, sfr::burnwire::mode);
}

void test_no_solar_current()
{
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::fault::check_temp_c = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
}

void test_armed_timeout()
{
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    delay(sfr::burnwire::armed_time);

    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
}

void test_fire_burnwire_logic()
{
    BurnwireControlTask burnwire_control_task(0);

    sfr::mission::mode = mission_mode_type::standby;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::armed, sfr::burnwire::mode);

    // Tests every combination of fire, temperature::in_sun, current::in_sun, check_temp_c, and check_solar_current

    // 11111
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = true;

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::camera::turn_on = false;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(true, sfr::camera::turn_on);

    // 11110
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = false;

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::camera::turn_on = false;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(true, sfr::camera::turn_on);

    // 11101
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = true;

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::camera::turn_on = false;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(true, sfr::camera::turn_on);

    // 11100
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = false;

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::camera::turn_on = false;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(true, sfr::camera::turn_on);

    // 11011
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = true;

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::camera::turn_on = false;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(true, sfr::camera::turn_on);

    // 11010
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = false;

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::camera::turn_on = false;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(true, sfr::camera::turn_on);

    // 11001
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 11000
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = false;

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::camera::turn_on = false;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(true, sfr::camera::turn_on);

    // 10111
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 10110
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = false;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 10101
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = true;

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::camera::turn_on = false;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(true, sfr::camera::turn_on);
    ;

    // 10100
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = false;

    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::camera::turn_on = false;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(burnwire_mode_type::fire, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(true, sfr::camera::turn_on);

    // 10011
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 10010
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = false;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 10001
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 10000
    sfr::burnwire::fire = true;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 01111
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 01110
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = false;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 01101
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 01100
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = false;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 01011
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 01010
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = false;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 01001
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 01000
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = true;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = false;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 00111
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 00110
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = false;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 00101
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 00100
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = true;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = false;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 00011
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 00010
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = true;
    sfr::fault::check_solar_current = false;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 00001
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = true;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));

    // 00000
    sfr::burnwire::fire = false;
    sfr::temperature::in_sun = false;
    sfr::current::in_sun = false;
    sfr::fault::check_temp_c = false;
    sfr::fault::check_solar_current = false;

    Pins::setPinState(constants::burnwire::first_pin, HIGH);
    Pins::setPinState(constants::burnwire::second_pin, HIGH);
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::burnwire::arm = true;
    sfr::burnwire::mode = burnwire_mode_type::armed;
    burnwire_control_task.execute();
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::first_pin));
    TEST_ASSERT_EQUAL(LOW, Pins::getPinState(constants::burnwire::second_pin));
}

int test_burnwire()
{
    UNITY_BEGIN();
    // Timeout is long- change in constants for testing
    // RUN_TEST(test_armed_timeout);
    RUN_TEST(test_valid_initialization);
    RUN_TEST(test_no_solar_current);
    RUN_TEST(test_out_of_order_commands);
    RUN_TEST(test_max_attempts);
    RUN_TEST(test_exit_deployment);
    RUN_TEST(test_sensor);
    RUN_TEST(test_camera_max_attempts);
    RUN_TEST(test_camera_some_attempts);
    RUN_TEST(test_fire_burnwire_logic);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_burnwire();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    test_burnwire();
}

void loop() {}
#endif
#include "MissionManager.hpp"
#include "sfr.hpp"
#include <unity.h>

void reset(){
    sfr::imu::gyro_z_average->set_value(0);
    sfr::imu::gyro_x_average->set_value(0);
    sfr::imu::gyro_y_average->set_value(0);
    sfr::aliveSignal::num_hard_faults = 0;
    sfr::aliveSignal::downlinked = false;
    sfr::detumble::start_time = millis();
    sfr::detumble::max_time = constants::time::two_hours;
    sfr::boot::max_time = constants::time::two_days;
    sfr::battery::voltage_average->set_value(sfr::battery::acceptable_battery+.1);
    sfr::rockblock::max_check_signal_time = constants::time::one_minute;
}

void reset_normal(MissionManager mission_manager, MissionMode *mode)
{
    reset();
    sfr::mission::current_mode = mode;
    sfr::battery::voltage_average->set_valid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(mode->get_id(), sfr::mission::current_mode->get_id());
}

void reset_lp(MissionManager mission_manager, MissionMode *mode)
{
    reset();
    sfr::mission::current_mode = mode;
    sfr::battery::voltage_average->set_invalid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(mode->get_id(), sfr::mission::current_mode->get_id());
}

void reset_transmit(MissionManager mission_manager, MissionMode *transmitMode){
    reset();
    sfr::mission::current_mode = transmitMode;
    sfr::battery::voltage_average->set_valid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(transmitMode->get_id(), sfr::mission::current_mode->get_id()); 
}

void reset(MissionManager mission_manager, MissionMode *mode){
    if(mode->get_type() == mode_type::TRANSMIT){
        reset_transmit(mission_manager, mode);
    } else if (mode->get_type() == mode_type::LP){
        reset_lp(mission_manager, mode);
    } else{
        reset_normal(mission_manager, mode);
    }
}

void test_exit_acs(MissionManager mission_manager, MissionMode *currentMode, MissionMode *nextMode){
    reset(mission_manager, currentMode);

    // do not exit if acs if time has not been reached
    sfr::acs::on_time = constants::time::one_hour;
    currentMode->set_start_time(millis());
    delay(5);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(currentMode->get_id(), sfr::mission::current_mode->get_id());

    reset(mission_manager, currentMode);
    
    // exit if time has been reached
    sfr::acs::on_time = 5;
    currentMode->set_start_time(millis());
    delay(sfr::acs::on_time);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->get_id(), sfr::mission::current_mode->get_id());
}

void test_enter_acs(MissionManager mission_manager, MissionMode *currentMode, MissionMode *nextMode){
    reset(mission_manager, currentMode);

    // stay in transmit mode if not enough time has passed
    sfr::mission::acs_transmit_cycle_time = constants::time::one_hour;
    sfr::acs::on_time = constants::time::one_second;
    delay(5);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(currentMode->get_id(), sfr::mission::current_mode->get_id());

    reset(mission_manager, currentMode);
    
    // exit transmit mode and enter normal mode if 
    sfr::mission::acs_transmit_cycle_time = 10*constants::time::one_second;
    sfr::acs::on_time = 5*constants::time::one_second;
    currentMode->set_start_time(millis());
    delay(sfr::mission::acs_transmit_cycle_time-sfr::acs::on_time);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->get_id(), sfr::mission::current_mode->get_id());
}

void test_exit_lp(MissionManager mission_manager, MissionMode *lpMode, MissionMode *currentMode){
    reset(mission_manager, currentMode);
    mission_manager.execute();
    reset(mission_manager, currentMode);

    // exit if voltage sensor resolves itself
    sfr::battery::voltage_average->set_valid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(currentMode->get_id(), sfr::mission::current_mode->get_id());

    reset(mission_manager, lpMode);

    // exit if battery voltage is high enough
    sfr::battery::voltage_average->set_value(sfr::battery::acceptable_battery+.1);
    sfr::battery::voltage_average->set_valid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(currentMode->get_id(), sfr::mission::current_mode->get_id());

    reset(mission_manager, lpMode);

    // don't exit if voltage sensor resolves itself but battery voltage does not
    sfr::battery::voltage_average->set_valid();
    sfr::battery::voltage_average->set_value(sfr::battery::min_battery-.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(lpMode->get_id(), sfr::mission::current_mode->get_id());

    reset(mission_manager, lpMode);

    // don't exit if voltage sensor resolves itself but battery voltage does not
    sfr::battery::voltage_average->set_invalid();
    sfr::battery::voltage_average->set_value(sfr::battery::acceptable_battery+.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(lpMode->get_id(), sfr::mission::current_mode->get_id());
}

void test_enter_lp(MissionManager mission_manager, MissionMode *lpMode, MissionMode *normalMode){
    reset(mission_manager, normalMode);

    // exit if voltage sensor fails
    sfr::battery::voltage_average->set_invalid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(lpMode->get_id(), sfr::mission::current_mode->get_id());

    reset(mission_manager, normalMode);

    // exit if battery voltage is too low
    sfr::battery::voltage_average->set_value(sfr::battery::min_battery-.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(lpMode->get_id(), sfr::mission::current_mode->get_id());

    reset(mission_manager, normalMode);

    // exit if battery voltage is too low and voltage sensor fails
    sfr::battery::voltage_average->set_invalid();
    sfr::battery::voltage_average->set_value(sfr::battery::min_battery-.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(lpMode->get_id(), sfr::mission::current_mode->get_id());
}

void test_exit_signal_phase(MissionManager mission_manager, MissionMode *currentMode, MissionMode *nextMode){
    reset(mission_manager, currentMode);
    sfr::aliveSignal::downlinked = false;
    
    // exit if downlinked
    sfr::aliveSignal::downlinked = true;
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->get_id(), sfr::mission::current_mode->get_id());

    reset(mission_manager, currentMode);

    //exit if reached max number of hard faults
    sfr::aliveSignal::num_hard_faults = sfr::aliveSignal::max_downlink_hard_faults;
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->get_id(), sfr::mission::current_mode->get_id());

    reset(mission_manager, currentMode);

    //exit if signal quality has not reached acceptable level in one minute
    sfr::rockblock::max_check_signal_time = 5;
    delay(sfr::rockblock::max_check_signal_time);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->get_id(), sfr::mission::current_mode->get_id());
    
    reset(mission_manager, currentMode);

    //don't exit if none of the above conditions are met
    mission_manager.execute();
    TEST_ASSERT_EQUAL(currentMode->get_id(), sfr::mission::current_mode->get_id());
}

void test_exit_detumble_phase(MissionManager mission_manager, MissionMode *currentMode, MissionMode *nextMode){
    reset(mission_manager, currentMode);

    // exit if detumbling times out
    sfr::detumble::start_time = millis();
    sfr::detumble::max_time = 5;
    delay(5);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->get_id(), sfr::mission::current_mode->get_id());

    reset(mission_manager, currentMode);

    //exit if imu determines stable
    sfr::imu::gyro_z_average->set_value(sfr::detumble::min_stable_gyro_z);
    sfr::imu::gyro_x_average->set_value(sfr::detumble::max_stable_gyro_x);
    sfr::imu::gyro_y_average->set_value(sfr::detumble::max_stable_gyro_y);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->get_id(), sfr::mission::current_mode->get_id());

    //exit if imu determines unstable
    sfr::imu::gyro_x_average->set_value(sfr::detumble::min_unstable_gyro_x);
    sfr::imu::gyro_y_average->set_value(sfr::detumble::min_unstable_gyro_y);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->get_id(), sfr::mission::current_mode->get_id());
}

void test_valid_initialization()
{
    MissionManager mission_manager(0);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::boot->get_id(), sfr::mission::current_mode->get_id());
}

void test_exit_boot()
{
    MissionManager mission_manager(0);
    reset(mission_manager, sfr::mission::boot);

    // after max boot time, transition to aliveSignal
    sfr::boot::max_time = 500;
    delay(sfr::boot::max_time);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::aliveSignal->get_id(), sfr::mission::current_mode->get_id());
}

void test_exit_alive_signal()
{
    MissionManager mission_manager(0);

    test_enter_lp(mission_manager, sfr::mission::lowPowerAliveSignal, sfr::mission::aliveSignal);
    test_exit_signal_phase(mission_manager, sfr::mission::aliveSignal, sfr::mission::detumbleSpin);
}


void test_exit_low_power_alive_signal()
{
    MissionManager mission_manager(0);

    test_exit_lp(mission_manager, sfr::mission::lowPowerAliveSignal, sfr::mission::aliveSignal);
    test_exit_signal_phase(mission_manager, sfr::mission::lowPowerAliveSignal, sfr::mission::lowPowerDetumbleSpin);
}


void test_exit_detumble_spin()
{
    MissionManager mission_manager(0);
    reset(mission_manager, sfr::mission::detumbleSpin);

    test_enter_lp(mission_manager, sfr::mission::lowPowerDetumbleSpin, sfr::mission::detumbleSpin);
    test_exit_detumble_phase(mission_manager, sfr::mission::detumbleSpin, sfr::mission::normal);
}

void test_exit_low_power_detumble_spin()
{
    MissionManager mission_manager(0);
    reset(mission_manager, sfr::mission::lowPowerDetumbleSpin);

    test_exit_lp(mission_manager, sfr::mission::lowPowerDetumbleSpin, sfr::mission::detumbleSpin);
    test_exit_detumble_phase(mission_manager, sfr::mission::lowPowerDetumbleSpin, sfr::mission::lowPower);
}

void test_standard_phase(MissionMode *normalMode, MissionMode *lpMode, MissionMode *transmitMode)
{
    //test exit normal
    MissionManager mission_manager(0);
    reset(mission_manager, normalMode);

    //test_enter_lp(mission_manager, lpMode, normalMode);
    test_exit_acs(mission_manager, normalMode, transmitMode);

    //test exit low power
    //if entered low power mode from normal mode go back to normal mode
    test_exit_lp(mission_manager, lpMode, normalMode);

    //if entered low power mode from transmit mode go back to transmit mode
    test_exit_lp(mission_manager, lpMode, transmitMode);

    //test exit transmit
    test_enter_lp(mission_manager, lpMode, transmitMode);
    test_enter_acs(mission_manager, transmitMode, normalMode);

}

void test_standby() {
    test_standard_phase(sfr::mission::normal, sfr::mission::lowPower, sfr::mission::transmit);
}

void test_deployment() {
    test_standard_phase(sfr::mission::normalDeployment, sfr::mission::lowPowerDeployment, sfr::mission::transmitDeployment);
}

void test_armed() {
    test_standard_phase(sfr::mission::normalArmed, sfr::mission::lowPowerArmed, sfr::mission::transmitArmed);
}
    

int test_mission_manager()
{
    UNITY_BEGIN();
    RUN_TEST(test_valid_initialization);
    RUN_TEST(test_exit_boot);
    RUN_TEST(test_exit_alive_signal);
    RUN_TEST(test_exit_low_power_alive_signal);
    RUN_TEST(test_exit_detumble_spin);
    RUN_TEST(test_exit_low_power_detumble_spin);
    RUN_TEST(test_standby);
    RUN_TEST(test_deployment);
    RUN_TEST(test_armed);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_mission_manager();
}
#else
#include <Arduino.h>
void setup()
{
    delay(5000);
    Serial.begin(9600);
    delay(5000);
    test_mission_manager();
}

void loop() {}
#endif
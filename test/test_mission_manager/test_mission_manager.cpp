#include "MissionManager.hpp"
#include "sfr.hpp"
#include <unity.h>

void reset(){
    sfr::imu::gyro_z_average->set_value(sfr::detumble::stable_gyro_z -1);
    sfr::aliveSignal::num_downlink_failures = 0;
    sfr::aliveSignal::downlinked = false;
    sfr::detumble::start_time = millis();
    sfr::detumble::max_time = constants::time::two_hours;
    sfr::boot::max_time = constants::time::two_days;
    sfr::battery::voltage_average->set_value(sfr::battery::acceptable_battery+.1);
}

void reset_normal(MissionManager mission_manager, MissionMode *mode)
{
    reset();
    sfr::mission::current_mode = mode;
    sfr::battery::voltage_average->set_valid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(mode->id, sfr::mission::current_mode->id);
}

void reset_lp(MissionManager mission_manager, MissionMode *mode)
{
    reset();
    sfr::mission::current_mode = mode;
    sfr::battery::voltage_average->set_invalid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(mode->id, sfr::mission::current_mode->id);
}

void reset_transmit(MissionManager mission_manager, MissionMode *transmitMode){
    reset();
    sfr::mission::current_mode = transmitMode;
    sfr::battery::voltage_average->set_valid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(transmitMode->id, sfr::mission::current_mode->id);

    sfr::aliveSignal::max_time = 500;
    delay(sfr::aliveSignal::max_time);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(transmitMode->id, sfr::mission::previous_mode->id);
    TEST_ASSERT_EQUAL(transmitMode->id, sfr::mission::current_mode->id);   
}

void conditional_reset(MissionManager mission_manager, MissionMode *lpMode, MissionMode *normalMode, bool lp){
    if(lp){
        reset_lp(mission_manager, lpMode);
    } else{
        reset_normal(mission_manager, normalMode);
    }
}

void test_acs_duty_cycle(MissionManager mission_manager, MissionMode *currentMode, MissionMode *nextMode){
    reset_normal(mission_manager, currentMode);

    // do not exit if acs if time has not been reached
    sfr::acs::on_time = constants::time::one_hour;
    currentMode->set_start_time(millis());
    delay(5);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(currentMode->id, sfr::mission::current_mode->id);

    reset_normal(mission_manager, currentMode);
    
    // exit if time has been reached
    sfr::acs::on_time = 5;
    currentMode->set_start_time(millis());
    delay(sfr::acs::on_time);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->id, sfr::mission::current_mode->id);
}

void test_exit_lp(MissionManager mission_manager, MissionMode *lpMode, MissionMode *normalMode){
    //reset_normal(mission_manager, normalMode);
    reset_transmit(mission_manager, sfr::mission::transmit);

    // exit if voltage sensor resolves itself
    sfr::battery::voltage_average->set_valid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(normalMode->id, sfr::mission::current_mode->id);

    reset_lp(mission_manager, lpMode);

    // exit if battery voltage is high enough
    sfr::battery::voltage_average->set_value(sfr::battery::acceptable_battery+.1);
    sfr::battery::voltage_average->set_valid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(normalMode->id, sfr::mission::current_mode->id);

    reset_lp(mission_manager, lpMode);

    // don't exit if voltage sensor resolves itself but battery voltage does not
    sfr::battery::voltage_average->set_valid();
    sfr::battery::voltage_average->set_value(sfr::battery::min_battery-.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(lpMode->id, sfr::mission::current_mode->id);

    reset_lp(mission_manager, lpMode);

    // don't exit if voltage sensor resolves itself but battery voltage does not
    sfr::battery::voltage_average->set_invalid();
    sfr::battery::voltage_average->set_value(sfr::battery::acceptable_battery+.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(lpMode->id, sfr::mission::current_mode->id);
}

void test_enter_lp(MissionManager mission_manager, MissionMode *lpMode, MissionMode *normalMode){
    reset_normal(mission_manager, normalMode);

    // exit if voltage sensor fails
    sfr::battery::voltage_average->set_invalid();
    mission_manager.execute();
    TEST_ASSERT_EQUAL(lpMode->id, sfr::mission::current_mode->id);

    reset_normal(mission_manager, normalMode);

    // exit if battery voltage is too low
    sfr::battery::voltage_average->set_value(sfr::battery::min_battery-.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(lpMode->id, sfr::mission::current_mode->id);

    reset_normal(mission_manager, normalMode);

    // exit if battery voltage is too low and voltage sensor fails
    sfr::battery::voltage_average->set_invalid();
    sfr::battery::voltage_average->set_value(sfr::battery::min_battery-.1);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(lpMode->id, sfr::mission::current_mode->id);
}

void test_exit_signal_phase(MissionManager mission_manager, bool lp){
    MissionMode *currentMode;
    MissionMode *nextMode;
    if(lp){
        currentMode = sfr::mission::lowPowerAliveSignal;
        nextMode = sfr::mission::lowPowerDetumbleSpin;
        reset_lp(mission_manager, currentMode);
    } else{
        currentMode = sfr::mission::aliveSignal;
        nextMode = sfr::mission::detumbleSpin;
        reset_normal(mission_manager, currentMode);
    }

    sfr::aliveSignal::downlinked = false;
    conditional_reset(mission_manager, sfr::mission::lowPowerAliveSignal, sfr::mission::aliveSignal, lp);

    // exit if downlinked
    sfr::aliveSignal::downlinked = true;
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->id, sfr::mission::current_mode->id);

    sfr::aliveSignal::downlinked = false;
    conditional_reset(mission_manager, sfr::mission::lowPowerAliveSignal, sfr::mission::aliveSignal, lp);

    //exit if reached max number of failed downlinks
    sfr::aliveSignal::num_downlink_failures = sfr::aliveSignal::max_downlink_failures;
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->id, sfr::mission::current_mode->id);
}

void test_exit_detumble_phase(MissionManager mission_manager, bool lp){
    MissionMode *currentMode;
    MissionMode *nextMode;
    if(lp){
        currentMode = sfr::mission::lowPowerDetumbleSpin;
        nextMode = sfr::mission::lowPower;
        reset_lp(mission_manager, currentMode);
    } else{
        currentMode = sfr::mission::detumbleSpin;
        nextMode = sfr::mission::normal;
        reset_normal(mission_manager, currentMode);
    }

    // exit if detumbling times out
    sfr::detumble::start_time = millis();
    sfr::detumble::max_time = 5;
    delay(5);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->id, sfr::mission::current_mode->id);

    conditional_reset(mission_manager, sfr::mission::lowPowerDetumbleSpin, sfr::mission::detumbleSpin, lp);

    //exit if imu determines stable
    sfr::imu::gyro_z_average->set_value(sfr::detumble::stable_gyro_z);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(nextMode->id, sfr::mission::current_mode->id);

}

void test_exit_acs(MissionManager mission_manager, MissionMode *transmitMode, MissionMode *normalMode){
    reset_normal(mission_manager, normalMode);

    test_acs_duty_cycle(mission_manager, normalMode, transmitMode);
}

void test_valid_initialization()
{
    MissionManager mission_manager(0);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::boot->id, sfr::mission::current_mode->id);
}

void test_exit_boot()
{
    MissionManager mission_manager(0);
    reset_normal(mission_manager, sfr::mission::boot);

    // after max boot time, transition to aliveSignal
    sfr::boot::max_time = 500;
    delay(sfr::boot::max_time);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::aliveSignal->id, sfr::mission::current_mode->id);
}

void test_exit_alive_signal()
{
    MissionManager mission_manager(0);

    test_enter_lp(mission_manager, sfr::mission::lowPowerAliveSignal, sfr::mission::aliveSignal);
    test_exit_signal_phase(mission_manager, false);

    // after max alive signal time, transition to detumble
    sfr::aliveSignal::max_time = 500;
    delay(sfr::aliveSignal::max_time);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::detumbleSpin->id, sfr::mission::current_mode->id);
}


void test_exit_low_power_alive_signal()
{
    MissionManager mission_manager(0);

    test_exit_lp(mission_manager, sfr::mission::lowPowerAliveSignal, sfr::mission::aliveSignal);
    test_exit_signal_phase(mission_manager, true);

     // after max alive signal time, transition to low power detumble
    /*sfr::aliveSignal::max_time = 500;
    delay(sfr::aliveSignal::max_time);
    mission_manager.execute();
    TEST_ASSERT_EQUAL(sfr::mission::lowPowerDetumbleSpin->id, sfr::mission::current_mode->id);*/
    reset_transmit(mission_manager, sfr::mission::transmit);
}


void test_exit_detumble_spin()
{
    MissionManager mission_manager(0);
    reset_normal(mission_manager, sfr::mission::detumbleSpin);

    test_enter_lp(mission_manager, sfr::mission::lowPowerDetumbleSpin, sfr::mission::detumbleSpin);
    test_exit_detumble_phase(mission_manager, false);
}

void test_exit_low_power_detumble_spin()
{
    MissionManager mission_manager(0);
    reset_lp(mission_manager, sfr::mission::lowPowerDetumbleSpin);

    test_exit_lp(mission_manager, sfr::mission::lowPowerDetumbleSpin, sfr::mission::detumbleSpin);
    test_exit_detumble_phase(mission_manager, true);
}

void test_exit_normal()
{
    MissionManager mission_manager(0);
    reset_normal(mission_manager, sfr::mission::normal);

    test_enter_lp(mission_manager, sfr::mission::lowPower, sfr::mission::normal);
    test_exit_acs(mission_manager, sfr::mission::transmit, sfr::mission::normal);

}

void test_exit_low_power(){
    MissionManager mission_manager(0);

    test_exit_lp(mission_manager, sfr::mission::lowPower, sfr::mission::normal);

    //if entered low power mode from transmit mode go back to transmit mode
    reset_normal(mission_manager, sfr::mission::transmit);
    test_exit_lp(mission_manager, sfr::mission::lowPower, sfr::mission::transmit);

}

void test_exit_transmit(){
    MissionManager mission_manager(0);

    test_enter_lp(mission_manager, sfr::mission::lowPower, sfr::mission::transmit);
    test_acs_duty_cycle(mission_manager, sfr::mission::transmit, sfr::mission::normal);
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
    RUN_TEST(test_exit_normal);
    RUN_TEST(test_exit_low_power);
    RUN_TEST(test_exit_transmit);
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
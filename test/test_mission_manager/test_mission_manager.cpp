#include <unity.h>
#include <MissionManager.hpp>
#include "Monitors/FaultMonitor.hpp"

void fault_monitor_test_helper_fault1();
void fault_monitor_test_helper_fault2();
/*
Helper test function for transition_to_standby
*/
void test_transition_to_standby_helper()
{
    //test transition_to_standby
    TEST_ASSERT_EQUAL(mission_mode_type::standby, sfr::mission::mode);
    TEST_ASSERT_EQUAL(fault_mode_type::active, sfr::fault::mode);
    TEST_ASSERT_EQUAL(acs_mode_type::full, sfr::acs::mode);
    TEST_ASSERT_EQUAL(temp_mode_type::active, sfr::temperature::mode);
    TEST_ASSERT_EQUAL(constants::rockblock::ten_minutes, sfr::rockblock::downlink_period);

    //test active fault_mode_type
    fault_monitor_test_helper_fault1();
    fault_monitor_test_helper_fault2();
}

/*
Helper test function for transition_to_safe
*/
void test_transition_to_safe_helper()
{
    //test transition_to_safe
    TEST_ASSERT_EQUAL(mission_mode_type::safe, sfr::mission::mode);
    TEST_ASSERT_EQUAL(acs_mode_type::simple, sfr::acs::mode);
    TEST_ASSERT_EQUAL(constants::rockblock::two_hours, sfr::rockblock::downlink_period);
    TEST_ASSERT_EQUAL(temp_mode_type::active, sfr::temperature::mode);
    TEST_ASSERT_EQUAL(fault_mode_type::inactive, sfr::fault::mode);
}

/*
Helper test function for fault_monitor when fault_mode_type is active and when FAULT_1
*/
void fault_monitor_test_helper_fault1()
{
    FaultMonitor fault_monitor(0);
    sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_x;
    fault_monitor.execute();
    test_transition_to_safe_helper();

    sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_y;
    fault_monitor.execute();
    test_transition_to_safe_helper();

    sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_z;
    fault_monitor.execute();
    test_transition_to_safe_helper();

    sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_x;
    fault_monitor.execute();
    test_transition_to_safe_helper();

    sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_y;
    fault_monitor.execute();
    test_transition_to_safe_helper();

    sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_z;
    fault_monitor.execute();
    test_transition_to_safe_helper();
}

/*
Helper test function for fault_monitor when fault_mode_type is active and when FAULT_2
*/
void fault_monitor_test_helper_fault2()
{
    FaultMonitor fault_monitor(0);
    sfr::fault::fault_2 = sfr::fault::fault_2 | constants::fault::temp_c;
    fault_monitor.execute();
    test_transition_to_safe_helper();

    sfr::fault::fault_2 = sfr::fault::fault_2 | constants::fault::solar_current;
    fault_monitor.execute();
    test_transition_to_safe_helper();

    sfr::fault::fault_2 = sfr::fault::fault_2 | constants::fault::voltage;
    fault_monitor.execute();
    test_transition_to_safe_helper();
}

/*
Test a valid initialization
*/
void test_valid_initialization()
{
    // transition_to_standby() got called inside
    MissionManager mission_manager(0);
    test_transition_to_standby_helper();
}

/*
Helper test function for transition_to_initialization
*/
void test_transition_to_initialization_helper()
{
    //test transition_to_initialization
    TEST_ASSERT_EQUAL(mission_mode_type::initialization, sfr::mission::mode);
    TEST_ASSERT_EQUAL(fault_mode_type::active, sfr::fault::mode);
    TEST_ASSERT_EQUAL(acs_mode_type::simple, sfr::acs::mode);
    TEST_ASSERT_EQUAL(temp_mode_type::active, sfr::temperature::mode);
    TEST_ASSERT_EQUAL(constants::rockblock::ten_minutes, sfr::rockblock::downlink_period);

    //test active fault_mode_type
    fault_monitor_test_helper_fault1();
    fault_monitor_test_helper_fault2();
}

/*
Helper test function for transition_to_low_power
*/
void test_transition_to_low_power_helper()
{
    //test transition_to_low_power
    TEST_ASSERT_EQUAL(mission_mode_type::low_power, sfr::mission::mode);
    TEST_ASSERT_EQUAL(fault_mode_type::active, sfr::fault::mode);
    TEST_ASSERT_EQUAL(acs_mode_type::off, sfr::acs::mode);
    TEST_ASSERT_EQUAL(constants::rockblock::two_hours, sfr::rockblock::downlink_period);
    TEST_ASSERT_EQUAL(temp_mode_type::inactive, sfr::temperature::mode);

    //test active fault_mode_type
    fault_monitor_test_helper_fault1();
    fault_monitor_test_helper_fault2();
}

/*
Helper test function for transition_to_deployment
*/
void test_transition_to_deployment_helper()
{
    //test transition_to_deployment
    TEST_ASSERT_EQUAL(true, sfr::camera::turn_off);
    TEST_ASSERT_EQUAL(mission_mode_type::deployment, sfr::mission::mode);
    TEST_ASSERT_EQUAL(acs_mode_type::simple, sfr::acs::mode);
    TEST_ASSERT_EQUAL(constants::rockblock::ten_minutes, sfr::rockblock::downlink_period);
    TEST_ASSERT_EQUAL(temp_mode_type::inactive, sfr::temperature::mode);
}

/*
Test valid dispatch initialization
*/
void test_valid_dispatch_initialization()
{
    MissionManager mission_manager(0);
    sfr::mission::mode = mission_mode_type::initialization;
    //ASSERT(sfr::rockblock::num_downlinks >= constants::rockblock::num_initial_downlinks);
    sfr::rockblock::num_downlinks = 3;
    mission_manager.execute();
    //Test for transition_to_standby();
    test_transition_to_standby_helper();
}

/*
Test valid dispatch standby
*/
void test_valid_dispatch_standby()
{
    MissionManager mission_manager(0);
    sfr::mission::mode = mission_mode_type::standby;
    //ASSERT(sfr::battery::voltage < 3.75 && sfr::fault::check_voltage && sfr::mission::low_power_eligible == true);
    sfr::battery::voltage = 3.0;
    sfr::fault::check_voltage = true;
    sfr::mission::low_power_eligible = true;
    mission_manager.execute();
    //Test for transition_to_low_power();
    test_transition_to_low_power_helper();
}

/*
Test valid dispatch safe
*/
void test_valid_dispatch_safe()
{
    MissionManager mission_manager(0);
    sfr::mission::mode = mission_mode_type::safe;
    mission_manager.execute();
    TEST_ASSERT_EQUAL(mission_mode_type::safe, sfr::mission::mode);
}

/*
Test valid dispatch low power
*/
void test_valid_dispatch_low_power()
{
    MissionManager mission_manager(0);
    //set mode to low power

    //Fault Check
    fault_monitor_test_helper_fault1();
    fault_monitor_test_helper_fault2();
    sfr::fault::fault_1 = 0;
    sfr::fault::fault_2 = 0;
    sfr::fault::fault_3 = 0;

    sfr::battery::voltage = 4.0;
    sfr::fault::check_voltage = true;
    sfr::mission::low_power_eligible = true;
    sfr::mission::mode = mission_mode_type::low_power;

    mission_manager.execute();
    Serial.println((int)sfr::mission::mode);
    mission_manager.execute();
    test_transition_to_standby_helper();
}

/*
Test valid dispatch deployment
*/
void test_valid_dispatch_deployment_FirstIf()
{
    MissionManager mission_manager(0);
    sfr::mission::mode = mission_mode_type::deployment;
    //ASSERT(sfr::battery::voltage < 3.75 && sfr::fault::check_voltage);
    sfr::battery::voltage = 3.0;
    sfr::fault::check_voltage = true;
    mission_manager.execute();
    //Test for transition_to_low_power();
    test_transition_to_low_power_helper();
}

void test_valid_dispatch_deployment_SecondIf()
{
    MissionManager mission_manager(0);
    sfr::mission::mode = mission_mode_type::deployment;
    //ASSERT(!sfr::button::pressed && !sfr::photoresistor::covered);
    sfr::button::pressed = false;
    sfr::photoresistor::covered = false;
    mission_manager.execute();
    //Test for sfr::camera::take_photo = true;
    TEST_ASSERT_EQUAL(true, sfr::camera::take_photo);
    //Test for BurnwireControlTask::transition_to_standby();
    TEST_ASSERT_EQUAL(burnwire_mode_type::standby, sfr::burnwire::mode);
    TEST_ASSERT_EQUAL(false, sfr::burnwire::fire);
    TEST_ASSERT_EQUAL(false, sfr::burnwire::arm);
    TEST_ASSERT_EQUAL(0, sfr::burnwire::attempts);
    //Test for transition_to_standby();
    test_transition_to_standby_helper();
}

void test_execute()
{
    //test constructors
    MissionManager mission_manager(0);
    FaultMonitor fault_monitor(0);
    // for constructor mission_manager
    test_transition_to_standby_helper();
    mission_manager.execute();
    fault_monitor_test_helper_fault1();
    fault_monitor_test_helper_fault2();
}

int test_mission_manager()
{
    UNITY_BEGIN();
    RUN_TEST(test_valid_dispatch_initialization);
    RUN_TEST(test_valid_dispatch_low_power);
    RUN_TEST(test_valid_dispatch_safe);
    RUN_TEST(test_valid_dispatch_standby);
    RUN_TEST(test_valid_dispatch_deployment_FirstIf);
    RUN_TEST(test_valid_dispatch_deployment_SecondIf);
    // RUN_TEST();
    RUN_TEST(test_execute);
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
    delay(2000);
    Serial.begin(9600);
    test_mission_manager();
}

void loop() {}
#endif

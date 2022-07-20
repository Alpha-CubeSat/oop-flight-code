#include <Monitors/CommandMonitor.hpp>
#include <unity.h>

void test_initialize()
{
    CommandMonitor command_monitor(0);
    TEST_ASSERT_EQUAL(0, 0);
}

// void test_mission()
// {
//     CommandMonitor command_monitor(0);

//     mission mode = deployment
//     sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
//     sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::deployment);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::mission::mode == mission_mode_type::deployment);

//     mission mode = standby
//     sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
//     sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::standby);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::mission::mode == mission_mode_type::standby);

//     mission mode = safe
//     sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
//     sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::safe);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::mission::mode == mission_mode_type::safe);

//     mission mode = low power
//     sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
//     sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::low_power);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::mission::mode == mission_mode_type::low_power);
// }

void test_burnwire()
{
    CommandMonitor command_monitor(0);

    // burnwire arm = on
    uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_arm);
    uint32_t f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::arm == true);

    // burnwire arm = off
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_arm);
    f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::arm == false);

    // burnwire fire = on
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_fire);
    f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::fire == true);

    // burnwire fire = off
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_fire);
    f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::fire == false);
}

void test_burn_times()
{
    CommandMonitor command_monitor(0);

    // burn time > max burn time
    uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_time);
    uint32_t f_arg_1 = 2 * constants::burnwire::max_burnwire_time;
    RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::burn_time == constants::time::half_second);

    // burn time in range
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_time);
    f_arg_1 = constants::time::one_second;
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::burn_time == constants::time::one_second);

    // armed time > max armed time
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_timeout);
    f_arg_1 = 2 * constants::burnwire::max_armed_time;
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::armed_time == 2 * constants::time::one_day);

    // arm time in range
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_timeout);
    f_arg_1 = constants::time::one_hour;
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::armed_time == constants::time::one_hour);
}

void test_camera()
{
    CommandMonitor command_monitor(0);

    // take photo = true
    uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::camera_take_photo);
    uint32_t f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::camera::take_photo == true);

    // take photo = false
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::camera_take_photo);
    f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::camera::take_photo == false);
}

// void test_acs_mode()
// {
//     CommandMonitor command_monitor(0);

//     acs mode = full
//     sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::acs_mode);
//     sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::full);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::acs::mode == acs_mode_type::full);

//     acs mode = simple
//     sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::acs_mode);
//     sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::simple);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::acs::mode == acs_mode_type::simple);

//     acs mode = off
//     sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::acs_mode);
//     sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::off);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::acs::mode == acs_mode_type::off);
// }

void test_fault_mode()
{
    CommandMonitor command_monitor(0);

    // faul mode = active
    uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_mode);
    uint32_t f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::active);
    RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::mode == fault_mode_type::active);

    // fault mode = inactive
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_mode);
    f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::inactive);
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::mode == fault_mode_type::inactive);
}

// void test_fault_mag()
// {
//     CommandMonitor command_monitor(0);

//     // check mag x = true
//     uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_x);
//     uint32_t f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
//     RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_mag_x == true);

//     // check mag x = false
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_x);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_mag_x == false);

//     // check mag y = true
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_y);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_mag_y == true);

//     // check mag y = false
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_y);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_mag_y == false);

//     // check mag z = true
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_z);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_mag_z == true);

//     // check mag z = false
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_z);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_mag_z == false);
// }

// void test_fault_gyro()
// {
//     CommandMonitor command_monitor(0);

//     // check gryo x = true
//     uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_x);
//     uint32_t f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
//     RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_gyro_x == true);

//     // check gryo x = false
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_x);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_gyro_x == false);

//     // check gryo y = true
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_y);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_gyro_y == true);

//     // check gryo y = false
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_y);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_gyro_y == false);

//     // check gryo z = true
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_z);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_gyro_z == true);

//     // check gryo z = false
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_z);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_gyro_z == false);
// }

// void test_fault_other()
// {
//     CommandMonitor command_monitor(0);

//     // check temp = true
//     uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_temp_c);
//     uint32_t f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
//     RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_temp_c == true);

//     // check temp = false
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_temp_c);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_temp_c == false);

//     // check current = true
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_solar_current);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_solar_current == true);

//     // check current = false
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_solar_current);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_solar_current == false);

//     // check voltage = true
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_voltage);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_voltage == true);

//     // check voltage = false
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_voltage);
//     f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::check_voltage == false);
// }

void test_img_frag()
{
    CommandMonitor command_monitor(0);

    // invalid fragment request
    uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::request_image_fragment);
    uint32_t f_arg_1 = 0;
    uint32_t f_arg_2 = 0;
    RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, f_arg_2);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::camera::fragment_requested == false);

    // valid fragment request
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::request_image_fragment);
    f_arg_1 = 1;
    f_arg_2 = 1;
    sfr::rockblock::camera_max_fragments[f_arg_1] = 2;
    command = RockblockCommand(f_opcode, f_arg_1, f_arg_2);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::camera::fragment_requested == true);
    TEST_ASSERT(sfr::camera::serial_requested == 1);
    TEST_ASSERT(sfr::camera::fragment_number_requested == 1);
}

void test_down_period()
{
    CommandMonitor command_monitor(0);

    // downlink period 2 hours
    uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::rockblock_downlink_period);
    uint32_t f_arg_1 = 2 * constants::time::one_hour;
    RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::rockblock::downlink_period == 2 * constants::time::one_hour);

    // downlink period too low (500 ms)
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::rockblock_downlink_period);
    f_arg_1 = constants::time::one_second / 2;
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::rockblock::downlink_period == 2 * constants::time::one_hour);

    // downlink period too high (4 days)
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::rockblock_downlink_period);
    f_arg_1 = 4 * constants::time::one_day;
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::rockblock::downlink_period == 2 * constants::time::one_hour);

    // downlink period in middle (1 hour)
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::rockblock_downlink_period);
    f_arg_1 = constants::time::one_hour;
    command = RockblockCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::rockblock::downlink_period == constants::time::one_hour);
}

void test_multiple_commands()
{
    CommandMonitor command_monitor(0);

    // downlink period 2 hours
    uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::rockblock_downlink_period);
    uint32_t f_arg_1 = constants::time::two_hours;
    uint32_t f_arg_2 = 0;
    RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, f_arg_2);
    sfr::rockblock::processed_commands.push_back(command);

    // valid fragment request
    f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::request_image_fragment);
    f_arg_1 = 1;
    f_arg_2 = 1;
    sfr::rockblock::camera_max_fragments[f_arg_1] = 2;
    command = RockblockCommand(f_opcode, f_arg_1, f_arg_2);
    sfr::rockblock::processed_commands.push_back(command);

    sfr::rockblock::waiting_command = true;
    command_monitor.execute();

    TEST_ASSERT(sfr::rockblock::downlink_period == 2 * constants::time::one_hour);
    TEST_ASSERT(sfr::camera::fragment_requested == true);
    TEST_ASSERT(sfr::camera::serial_requested == 1);
    TEST_ASSERT(sfr::camera::fragment_number_requested == 1);
}

int test_command_monitor()
{
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    // RUN_TEST(test_mission);
    RUN_TEST(test_burnwire);
    RUN_TEST(test_burn_times);
    RUN_TEST(test_camera);
    // RUN_TEST(test_acs_mode);
    RUN_TEST(test_fault_mode);
    // RUN_TEST(test_fault_mag);
    // RUN_TEST(test_fault_gyro);
    // RUN_TEST(test_fault_other);
    RUN_TEST(test_img_frag);
    RUN_TEST(test_down_period);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_simulator();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    test_command_monitor();
}

void loop() {}
#endif
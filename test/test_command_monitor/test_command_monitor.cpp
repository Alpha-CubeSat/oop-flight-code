#include <BurnwireCommands.hpp>
#include <Monitors/CommandMonitor.hpp>
#include <unity.h>

void test_initialize()
{
    CommandMonitor command_monitor(0);
    TEST_ASSERT_EQUAL(0, 0);
}

void test_special_commands()
{
    CommandMonitor command_monitor(0);
    sfr::rockblock::waiting_command = true;

    uint16_t f_opcode = RockblockCommand::get_decimal_opcode((u_int8_t *)constants::rockblock::opcodes::sfr_field_opcode_arm);
    // burnwire arm = on
    RockblockCommand *arm_on_command = new ArmCommand(f_opcode, 1, 0);
    sfr::rockblock::processed_commands.push_back((RockblockCommand *)arm_on_command);
    command_monitor.execute();
    TEST_ASSERT(sfr::mission::current_mode == sfr::mission::normalArmed);
    // burnwire arm = off
    RockblockCommand *arm_off_command = new ArmCommand(f_opcode, 1, 0);
    sfr::rockblock::processed_commands.push_back((RockblockCommand *)arm_off_command);
    command_monitor.execute();
    TEST_ASSERT(sfr::mission::current_mode == sfr::mission::normalArmed);

    f_opcode = RockblockCommand::get_decimal_opcode((u_int8_t *)constants::rockblock::opcodes::sfr_field_opcode_fire);
    // burnwire fire = on
    RockblockCommand *fire_on_command = new FireCommand(f_opcode, 1, 0);
    sfr::rockblock::processed_commands.push_back((RockblockCommand *)fire_on_command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::mission::current_mode == sfr::mission::normalInSun);

    // burnwire fire = off
    RockblockCommand *fire_off_command = new FireCommand(f_opcode, 0, 0);
    sfr::rockblock::processed_commands.push_back((RockblockCommand *)fire_off_command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::mission::current_mode == sfr::mission::normalInSun);

    f_opcode = RockblockCommand::get_decimal_opcode((u_int8_t *)constants::rockblock::opcodes::sfr_field_opcode_deploy);
    // test deploy
    RockblockCommand *deploy_on_command = new DeployCommand(f_opcode, 0, 0);
    sfr::rockblock::processed_commands.push_back((RockblockCommand *)deploy_on_command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::mission::current_mode == sfr::mission::normalDeployment);
}

// Test overide sfr field
// since opcode that are out of range will never be added to the queue and executed, we don't test UnknownCommand()

void test_override_sfr_burnwire()
{
    CommandMonitor command_monitor(0);

    // override burnwire burn time
    uint16_t f_opcode = 0x1902;
    uint32_t f_arg_1 = constants::time::one_second;
    RockblockCommand *short_command = new SFROverrideCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(short_command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::burn_time == constants::time::one_second);

    // override burwire armed time
    f_opcode = 0x1903;
    f_arg_1 = constants::time::one_hour;
    RockblockCommand *long_command = new SFROverrideCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(long_command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::armed_time == constants::time::one_hour);
}

void test_camera()
{
    CommandMonitor command_monitor(0);

    // take photo = true
    uint16_t f_opcode = 0x2001;
    uint32_t f_arg_1 = true;
    RockblockCommand *true_command = new SFROverrideCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(true_command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::camera::take_photo == true);

    // take photo = false
    f_opcode = 0x2001;
    f_arg_1 = false;
    RockblockCommand *false_command = new SFROverrideCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(false_command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::camera::take_photo == false);
}

void test_acs_mode()
{
    CommandMonitor command_monitor(0);

    // acs max_no_communication
    uint16_t f_opcode = 0x2500;
    uint32_t f_arg_1 = 11;
    RockblockCommand *max_no_communication_command = new SFROverrideCommand(f_opcode, f_arg_1, 0);
    sfr::rockblock::processed_commands.push_back(max_no_communication_command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::acs::max_no_communication == 11);

    // acs on time
    f_opcode = 0x2501;
    uint32_t f_arg_time = constants::time::one_minute;
    RockblockCommand *on_time_command = new SFROverrideCommand(f_opcode, f_arg_time, 0);
    sfr::rockblock::processed_commands.push_back(on_time_command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::acs::on_time == constants::time::one_minute);

    // acs off
    f_opcode = 0x2502;
    uint32_t f_arg_false = false;
    RockblockCommand *off_command = new SFROverrideCommand(f_opcode, f_arg_false, 0);
    sfr::rockblock::processed_commands.push_back(off_command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::acs::off == false);

    // acs mag
    f_opcode = 0x2503;
    uint32_t f_arg_acs = (uint16_t)simple_acs_type::y;
    RockblockCommand *mag_command = new SFROverrideCommand(f_opcode, f_arg_acs, 0);
    sfr::rockblock::processed_commands.push_back(mag_command);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::acs::mag == (uint16_t)simple_acs_type::y);
}

// void test_fault_mode()
// {
//     CommandMonitor command_monitor(0);

//     // faul mode = active
//     uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_mode);
//     uint32_t f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::active);
//     RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::mode == fault_mode_type::active);

//     // fault mode = inactive
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_mode);
//     f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::inactive);
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::fault::mode == fault_mode_type::inactive);
// }

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

// faults are irrelavant for sfr
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
//     TEST_ASSERT(constants::fault::gyro_x == false);

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

// void test_img_frag()
// {
//     CommandMonitor command_monitor(0);

//     // invalid fragment request
//     uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::request_image_fragment);
//     uint32_t f_arg_1 = 0;
//     uint32_t f_arg_2 = 0;
//     RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, f_arg_2);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::camera::fragment_requested == false);

//     // valid fragment request
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::request_image_fragment);
//     f_arg_1 = 1;
//     f_arg_2 = 1;
//     sfr::rockblock::camera_max_fragments[f_arg_1] = 2;
//     command = RockblockCommand(f_opcode, f_arg_1, f_arg_2);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::camera::fragment_requested == true);
//     TEST_ASSERT(sfr::camera::serial_requested == 1);
//     TEST_ASSERT(sfr::camera::fragment_number_requested == 1);
// }

// void test_down_period()
// {
//     CommandMonitor command_monitor(0);

//     // downlink period 2 hours
//     uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::rockblock_downlink_period);
//     uint32_t f_arg_1 = 2 * constants::time::one_hour;
//     RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::rockblock::downlink_period == 2 * constants::time::one_hour);

//     // downlink period too low (500 ms)
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::rockblock_downlink_period);
//     f_arg_1 = constants::time::one_second / 2;
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::rockblock::downlink_period == 2 * constants::time::one_hour);

//     // downlink period too high (4 days)
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::rockblock_downlink_period);
//     f_arg_1 = 4 * constants::time::one_day;
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::rockblock::downlink_period == 2 * constants::time::one_hour);

//     // downlink period in middle (1 hour)
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::rockblock_downlink_period);
//     f_arg_1 = constants::time::one_hour;
//     command = RockblockCommand(f_opcode, f_arg_1, 0);
//     sfr::rockblock::processed_commands.push_back(command);
//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();
//     TEST_ASSERT(sfr::rockblock::downlink_period == constants::time::one_hour);
// }

// void test_multiple_commands()
// {
//     CommandMonitor command_monitor(0);

//     // downlink period 2 hours
//     uint16_t f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::rockblock_downlink_period);
//     uint32_t f_arg_1 = constants::time::two_hours;
//     uint32_t f_arg_2 = 0;
//     RockblockCommand command = RockblockCommand(f_opcode, f_arg_1, f_arg_2);
//     sfr::rockblock::processed_commands.push_back(command);

//     // valid fragment request
//     f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::request_image_fragment);
//     f_arg_1 = 1;
//     f_arg_2 = 1;
//     sfr::rockblock::camera_max_fragments[f_arg_1] = 2;
//     command = RockblockCommand(f_opcode, f_arg_1, f_arg_2);
//     sfr::rockblock::processed_commands.push_back(command);

//     sfr::rockblock::waiting_command = true;
//     command_monitor.execute();

//     TEST_ASSERT(sfr::rockblock::downlink_period == 2 * constants::time::one_hour);
//     TEST_ASSERT(sfr::camera::fragment_requested == true);
//     TEST_ASSERT(sfr::camera::serial_requested == 1);
//     TEST_ASSERT(sfr::camera::fragment_number_requested == 1);
// }

int test_command_monitor()
{
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_override_sfr_burnwire);
    // // RUN_TEST(test_mission);
    RUN_TEST(test_special_commands);
    RUN_TEST(test_camera);
    RUN_TEST(test_acs_mode);
    // RUN_TEST(test_burn_times);
    // RUN_TEST(test_camera);
    // // RUN_TEST(test_acs_mode);
    // RUN_TEST(test_fault_mode);
    // // RUN_TEST(test_fault_mag);
    // // RUN_TEST(test_fault_gyro);
    // // RUN_TEST(test_fault_other);
    // RUN_TEST(test_img_frag);
    // RUN_TEST(test_down_period);
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
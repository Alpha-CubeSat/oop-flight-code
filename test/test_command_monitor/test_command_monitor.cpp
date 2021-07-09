#include <unity.h>
#include <Monitors/CommandMonitor.hpp>

void test_initialize() {
    CommandMonitor command_monitor(0);
    TEST_ASSERT_EQUAL(0, 0);
}

void test_mission() {
    CommandMonitor command_monitor(0);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::deployment);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::mission::mode == mission_mode_type::deployment);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::standby);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::mission::mode == mission_mode_type::standby);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::safe);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::mission::mode == mission_mode_type::safe);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::mission_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::low_power);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::mission::mode == mission_mode_type::low_power);
}

void test_burnwire() {
    CommandMonitor command_monitor(0);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_arm);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::arm == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_arm);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::arm == false);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_arm);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::arm == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::burnwire_arm);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::burnwire::arm == false);
}

void test_camera() {
    CommandMonitor command_monitor(0);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::camera_take_photo);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::camera::take_photo == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::camera_take_photo);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::camera::take_photo == false);
}

void test_temp_mode() {
    CommandMonitor command_monitor(0);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::temperature_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::active);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::temperature::mode == temp_mode_type::active);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::temperature_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::inactive);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::temperature::mode == temp_mode_type::inactive);
}

void test_acs_mode() {
    CommandMonitor command_monitor(0);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::acs_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::full);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::acs::mode == acs_mode_type::full);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::acs_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::simple);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::acs::mode == acs_mode_type::simple);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::acs_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::off);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::acs::mode == acs_mode_type::off);
}

void test_fault_mode() {
    CommandMonitor command_monitor(0);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::active);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::mode == fault_mode_type::active);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_mode);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_arg(constants::rockblock::inactive);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::mode == fault_mode_type::inactive);
}

void test_fault_mag() {
    CommandMonitor command_monitor(0);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_x);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_mag_x == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_x);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_mag_x == false);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_y);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_mag_y == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_y);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_mag_y == false);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_z);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_mag_z == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_mag_z);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_mag_z == false);
}

void test_fault_gyro() {
    CommandMonitor command_monitor(0);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_x);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_gyro_x == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_x);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_gyro_x == false);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_y);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_gyro_y == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_y);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_gyro_y == false);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_z);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_gyro_z == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_gyro_z);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_gyro_z == false);
}

void test_fault_other() {
    CommandMonitor command_monitor(0);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_temp_c);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_temp_c == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_temp_c);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_temp_c == false);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_solar_current);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_solar_current == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_solar_current);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_solar_current == false);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_voltage);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::true_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_voltage == true);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::fault_check_voltage);
    sfr::rockblock::f_arg_1 = command_monitor.get_decimal_opcode(constants::rockblock::false_arg);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::fault::check_voltage == false);
}

void test_img_frag() {
    CommandMonitor command_monitor(0);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::request_image_fragment);
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::camera::fragment_requested == true);
    // other fragment code checks TBD
}

void test_downlink_period() {
    CommandMonitor command_monitor(0);

    sfr::rockblock::f_opcode = command_monitor.get_decimal_opcode(constants::rockblock::rockblock_downlink_period);
    sfr::rockblock::f_arg_1 = 1;
    sfr::rockblock::waiting_command = true;
    command_monitor.execute();
    TEST_ASSERT(sfr::rockblock::downlink_period == 1);
    // max and min TBD
}

int test_command_monitor() {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_mission);
    RUN_TEST(test_burnwire);
    RUN_TEST(test_camera);
    RUN_TEST(test_temp_mode);
    RUN_TEST(test_acs_mode);
    RUN_TEST(test_fault_mode);
    RUN_TEST(test_fault_mag);
    RUN_TEST(test_fault_gyro);
    RUN_TEST(test_fault_other);
    // RUN_TEST(test_img_frag); // incomplete
    // RUN_TEST(test_downlink_period); // incomplete
    return UNITY_END();
}

#ifdef DESKTOP
int main() {
    return test_simulator();
}
#else
#include <Arduino.h>
void setup() {
    delay(2000);
    Serial.begin(9600);
    test_command_monitor();
}

void loop() {}
#endif
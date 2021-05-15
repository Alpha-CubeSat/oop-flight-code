#include <unity.h>
#include <Monitors/CameraReportMonitor.hpp>
#include <Control Tasks/CameraControlTask.hpp>

void test_camera_valid_initialize(){
    CameraControlTask camera_control_task(0);
    TEST_ASSERT_EQUAL(false, sfr::camera::powered);
}

void test_camera_power_on(){
    CameraControlTask camera_control_task(0);
    TEST_ASSERT_EQUAL(false, sfr::camera::powered);

    sfr::camera::turn_on = true;
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(true, sfr::camera::powered);
}

void test_camera_power_off(){
    CameraControlTask camera_control_task(0);
    TEST_ASSERT_EQUAL(false, sfr::camera::powered);

    sfr::camera::turn_on = true;
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(true, sfr::camera::powered);

    sfr::camera::turn_on = false;
    sfr::camera::turn_off = true;
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(false, sfr::camera::powered);
}

void test_take_picture(){
    CameraControlTask camera_control_task(0);
    TEST_ASSERT_EQUAL(false, sfr::camera::powered);

    sfr::camera::turn_on = true;
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(true, sfr::camera::powered);

    sfr::camera::take_photo = true;
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(true, sfr::camera::report_ready);
    TEST_ASSERT_EQUAL(1, sfr::camera::images_written);
}

void test_camera_report_prepare(){
    CameraReportMonitor camera_report_monitor(0);
    sfr::camera::report_ready = true;
    camera_report_monitor.execute();
    TEST_ASSERT_EQUAL(false, sfr::camera::report_ready);
    TEST_ASSERT_EQUAL(sfr::camera::current_serial, sfr::camera::images_written);
}

int test_camera() {
    UNITY_BEGIN();
    RUN_TEST(test_camera_valid_initialize);
    RUN_TEST(test_camera_power_on);
    RUN_TEST(test_camera_power_off);
    RUN_TEST(test_take_picture);
    RUN_TEST(test_camera_report_prepare);
    return UNITY_END();
}

#include <Arduino.h>
void setup() {
    delay(2000);
    Serial.begin(9600);
    test_camera();
}

void loop() {}
int main() { int test = 1; return test;}
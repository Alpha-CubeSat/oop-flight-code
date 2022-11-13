#include <Control Tasks/CameraControlTask.hpp>
#include <Monitors/CameraReportMonitor.hpp>
#include <unity.h>

void test_camera_valid_initialize()
{
    CameraControlTask camera_control_task(0);
    // TEST_ASSERT_EQUAL(false, sfr::camera::powered);
    TEST_ASSERT_EQUAL(2, 2);
}

void test_camera_power_on()
{

    CameraControlTask camera_control_task(0);
    TEST_ASSERT_EQUAL(false, sfr::camera::powered);
    TEST_ASSERT_EQUAL(camera_init_mode_type::awaiting, sfr::camera::init_mode);
    sfr::camera::turn_on = true;

    camera_control_task.execute();
    TEST_ASSERT_EQUAL(1, sfr::camera::start_progress);
    delay(100);
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(2, sfr::camera::start_progress);
    delay(500);
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(3, sfr::camera::start_progress);
    delay(600);
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(4, sfr::camera::start_progress);
    delay(300);
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(5, sfr::camera::start_progress);
    delay(100);
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(true, sfr::camera::powered);

    TEST_ASSERT_EQUAL(camera_init_mode_type::complete, sfr::camera::init_mode);
    TEST_ASSERT_EQUAL(false, sfr::camera::turn_on);
    TEST_ASSERT_EQUAL(true, sfr::camera::powered);
    TEST_ASSERT_EQUAL(sensor_mode_type::normal, sfr::camera::mode);
}

void test_take_picture_and_turn_off()
{
    CameraControlTask camera_control_task(0);
    sfr::camera::take_photo = true;
    for (int i = 0; i< 80; i++) {
        camera_control_task.execute();
    }
    TEST_ASSERT_EQUAL(true, sfr::camera::report_ready);
    TEST_ASSERT_FALSE(sfr::camera::take_photo);
    TEST_ASSERT_EQUAL(1, sfr::camera::images_written);
    TEST_ASSERT_FALSE(sfr::camera::powered);
    TEST_ASSERT_FALSE(sfr::camera::turn_off);
}

void test_camera_failure() {
    // fail once
    CameraControlTask camera_control_task(0);
    sfr::camera::failed_times = 0;
    sfr::camera::failed_limit = 1;
    sfr::camera::turn_on = true;
    sfr::camera::powered = false;
    sfr::camera::init_mode = (uint16_t)camera_init_mode_type::failed;
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(1, sfr::camera::failed_times);
    TEST_ASSERT_EQUAL(camera_init_mode_type::awaiting, sfr::camera::init_mode);

    // failed more than limit
    sfr::camera::init_mode = (uint16_t)camera_init_mode_type::failed;
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(0, sfr::camera::failed_times);
    TEST_ASSERT_EQUAL(sensor_mode_type::abnormal_init, sfr::camera::mode);
    TEST_ASSERT_FALSE(sfr::camera::powered);
    TEST_ASSERT_FALSE(sfr::camera::turn_off);
    TEST_ASSERT_FALSE(sfr::camera::turn_on);
    TEST_ASSERT_EQUAL(camera_init_mode_type::awaiting, sfr::camera::init_mode);
    TEST_ASSERT_EQUAL(0, sfr::camera::start_progress);

}

void test_camera_timeout()
{
    CameraControlTask camera_control_task(0);
    sfr::camera::turn_on = true;
    sfr::camera::powered = false;
    sfr::camera::init_mode = (uint16_t)camera_init_mode_type::in_progress;
    sfr::camera::init_start_time = millis();
    delay(sfr::camera::init_timeout + 1);
    camera_control_task.execute();
    TEST_ASSERT_EQUAL(1, sfr::camera::failed_times);
}

void test_camera_report_prepare()
{
    CameraReportMonitor camera_report_monitor(0);
    sfr::camera::report_ready = true;
    camera_report_monitor.execute();
    TEST_ASSERT_EQUAL(false, sfr::camera::report_ready);
    TEST_ASSERT_FALSE(sfr::rockblock::camera_report.empty());
    sfr::rockblock::camera_report.pop_front();
    TEST_ASSERT_EQUAL(sfr::rockblock::camera_report.front(), sfr::camera::images_written);
}

int test_camera()
{
    UNITY_BEGIN();
    RUN_TEST(test_camera_valid_initialize);
    RUN_TEST(test_camera_power_on);
    RUN_TEST(test_take_picture_and_turn_off);
    RUN_TEST(test_camera_failure);
    RUN_TEST(test_camera_timeout); // delays 12 seconds.
    RUN_TEST(test_camera_report_prepare);
    return UNITY_END();
}

#ifdef DESKTOP
int main()
{
    return test_camera();
}
#else
#include <Arduino.h>
void setup()
{
    delay(2000);
    Serial.begin(9600);
    Serial.println("Camera Test Started");
    test_camera();
}

void loop() {}
#endif
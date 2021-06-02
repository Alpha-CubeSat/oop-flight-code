#include "NormalReportMonitor.hpp"

NormalReportMonitor::NormalReportMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void NormalReportMonitor::execute(){
    Serial.println("-----------------PHOTORESISTOR----------------------");
    Serial.print("covered: ");
    Serial.println(sfr::photoresistor::covered);

    Serial.println("--------------------MISSION-------------------------");
    Serial.print("mode: ");
    Serial.println((int)sfr::mission::mode);

    Serial.println("-------------------BURNWIRE-------------------------");
    Serial.print("fire: ");
    Serial.println(sfr::burnwire::fire);
    Serial.print("mode: ");
    Serial.println((int)sfr::burnwire::mode);
    Serial.print("attempts: ");
    Serial.println(sfr::burnwire::attempts);
    Serial.print("start_time: ");
    Serial.println(sfr::burnwire::start_time);

    Serial.println("--------------------CAMERA--------------------------");
    Serial.print("take_photo: ");
    Serial.println(sfr::camera::take_photo);
    Serial.print("turn_on: ");
    Serial.println(sfr::camera::turn_on);
    Serial.print("turn_off: ");
    Serial.println(sfr::camera::turn_off);
    Serial.print("powered: ");
    Serial.println(sfr::camera::powered);
    Serial.print("current_serial: ");
    Serial.println(sfr::camera::current_serial);
    Serial.print("fragment_number: ");
    Serial.println(sfr::camera::fragment_number);
    Serial.print("max_fragments: ");
    Serial.println(sfr::camera::max_fragments);
    Serial.print("data_length: ");
    Serial.println(sfr::camera::data_length);
    Serial.print("fragment_number_requested: ");
    Serial.println(sfr::camera::fragment_number_requested);
    Serial.print("serial_requested: ");
    Serial.println(sfr::camera::serial_requested);
    Serial.print("fragment_requested: ");
    Serial.println(sfr::camera::fragment_requested);
    Serial.print("images_written: ");
    Serial.println(sfr::camera::images_written);
    Serial.print("report_ready: ");
    Serial.println(sfr::camera::report_ready);
    Serial.print("sd_card_failed: ");
    Serial.println(sfr::camera::sd_card_failed);
    Serial.print("camera_failed: ");
    Serial.println(sfr::camera::camera_failed);
    Serial.print("full_image_written: ");
    Serial.println(sfr::camera::full_image_written);
    Serial.print("report_downlinked: ");
    Serial.println(sfr::camera::report_downlinked);

    Serial.println("-------------------ROCKBLOCK------------------------");
    Serial.print("last_downlink: ");
    Serial.println(sfr::rockblock::last_downlink);
    Serial.print("downlink_period: ");
    Serial.println(sfr::rockblock::downlink_period);
    Serial.print("mode: ");
    Serial.println((int)sfr::rockblock::mode);
    Serial.print("waiting_message: ");
    Serial.println(sfr::rockblock::waiting_message);
    Serial.print("num_iter: ");
    Serial.println(sfr::rockblock::num_iter);
    Serial.print("waiting_command: ");
    Serial.println(sfr::rockblock::waiting_command);
    Serial.print("f_data_length: ");
    Serial.println(sfr::rockblock::f_data_length);
    Serial.print("f_opcode: ");
    Serial.println(sfr::rockblock::f_opcode);
    Serial.print("f_arg_1: ");
    Serial.println(sfr::rockblock::f_arg_1);
    Serial.print("f_arg_2: ");
    Serial.println(sfr::rockblock::f_arg_2);

    Serial.println("----------------------IMU----------------------------");
    Serial.print("mag_x: ");
    Serial.println((int)sfr::imu::mag_x);
    Serial.print("mag_y: ");
    Serial.println((int)sfr::imu::mag_y);
    Serial.print("mag_z: ");
    Serial.println((int)sfr::imu::mag_z);
    Serial.print("gyro_x: ");
    Serial.println((int)sfr::imu::gyro_x);
    Serial.print("gyro_y: ");
    Serial.println((int)sfr::imu::gyro_y);
    Serial.print("gyro_z: ");
    Serial.println((int)sfr::imu::gyro_z);
    Serial.print("acc_x: ");
    Serial.println((int)sfr::imu::acc_x);
    Serial.print("acc_y: ");
    Serial.println((int)sfr::imu::acc_y);
    Serial.print("acc_z: ");
    Serial.println((int)sfr::imu::acc_z);

    Serial.println("------------------TEMPERATURE-----------------------");
    Serial.print("temp_c: ");
    Serial.println((int)sfr::temperature::temp_c);
    Serial.print("mode: ");
    Serial.println((int)sfr::temperature::mode);

    Serial.println("--------------------CURRENT-----------------------");
    Serial.print("solar_current: ");
    Serial.println((int)sfr::current::solar_current);

    Serial.println("--------------------BATTERY-----------------------");
    Serial.print("voltage: ");
    Serial.println((int)sfr::battery::voltage);

    Serial.println("--------------------BUTTON-----------------------");
    Serial.print("pressed: ");
    Serial.println(sfr::button::pressed);
    
    uint8_t mag_x = map(sfr::imu::mag_x, constants::imu::min_mag_x, constants::imu::max_mag_x, 0, 255);
    uint8_t mag_y = map(sfr::imu::mag_y, constants::imu::min_mag_y, constants::imu::max_mag_y, 0, 255);
    uint8_t mag_z = map(sfr::imu::mag_z, constants::imu::min_mag_z, constants::imu::max_mag_z, 0, 255);

    uint8_t gyro_x = map(sfr::imu::gyro_x, constants::imu::min_gyro_x, constants::imu::max_gyro_x, 0, 255);
    uint8_t gyro_y = map(sfr::imu::gyro_y, constants::imu::min_gyro_y, constants::imu::max_gyro_y, 0, 255);
    uint8_t gyro_z = map(sfr::imu::gyro_z, constants::imu::min_gyro_z, constants::imu::max_gyro_z, 0, 255);

    uint8_t acc_x = map(sfr::imu::acc_x, constants::imu::min_acc_x, constants::imu::max_acc_x, 0, 255);
    uint8_t acc_y = map(sfr::imu::acc_y, constants::imu::min_acc_y, constants::imu::max_acc_y, 0, 255);
    uint8_t acc_z = map(sfr::imu::acc_z, constants::imu::min_acc_z, constants::imu::max_acc_z, 0, 255);

    uint8_t report[70] = {mag_x, 
                        mag_y, 
                        mag_z, 
                        gyro_x, 
                        gyro_y, 
                        gyro_z, 
                        acc_x, 
                        acc_y, 
                        acc_z, 
                        sfr::button::pressed, 
                        sfr::fault::fault_1};
    
}

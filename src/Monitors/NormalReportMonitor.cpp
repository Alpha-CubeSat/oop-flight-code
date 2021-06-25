#include "NormalReportMonitor.hpp"

NormalReportMonitor::NormalReportMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void NormalReportMonitor::execute(){

    #ifdef PRINT
    Serial.println("-----------------PHOTORESISTOR----------------------");
    Serial.print("covered: ");
    Serial.println(sfr::photoresistor::covered);

    Serial.println("--------------------MISSION-------------------------");
    Serial.print("mode: ");
    switch((int)sfr::mission::mode){
        case 0:
            Serial.println("low_power");
            break;
        case 1:
            Serial.println("deployment");
            break;
        case 2:
            Serial.println("standby");
            break;
        case 3:
            Serial.println("safe");
            break;
    }

    Serial.println("-------------------BURNWIRE-------------------------");
    Serial.print("fire: ");
    Serial.println(sfr::burnwire::fire);
    Serial.print("mode: ");
    switch((int)sfr::burnwire::mode){
        case 0:
            Serial.println("standby");
            break;
        case 1:
            Serial.println("burn");
            break;
        case 2:
            Serial.println("delay");
            break;
    }
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
    switch((int)sfr::rockblock::mode){
        case 0:
            Serial.println("standby");
            break;
        case 1:
            Serial.println("send_at");
            break;
        case 2:
            Serial.println("await_at");
            break;
        case 3:
            Serial.println("send_signal_strength");
            break;
        case 4:
            Serial.println("await_signal_strength");
            break;
        case 5:
            Serial.println("send_flow_control");
            break;
        case 6:
            Serial.println("await_flow_control");
            break;
        case 7:
            Serial.println("send_message_length");
            break;
        case 8:
            Serial.println("await_message_length");
            break;
        case 9:
            Serial.println("send_message");
            break;
        case 10:
            Serial.println("await_message");
            break;
        case 11:
            Serial.println("send_response");
            break;
        case 12:
            Serial.println("create_buffer");
            break;
        case 13:
            Serial.println("process_mo_status");
            break;
        case 14:
            Serial.println("process_mt_status");
            break;
        case 15:
            Serial.println("read_message");
            break;
        case 16:
            Serial.println("process_command");
            break;
        case 17:
            Serial.println("end_transmission");
            break;
    }
    Serial.print("waiting_message: ");
    Serial.println(sfr::rockblock::waiting_message);
    Serial.print("num_iter: ");
    Serial.println(sfr::rockblock::num_iter);
    Serial.print("waiting_command: ");
    Serial.println(sfr::rockblock::waiting_command);
    Serial.print("f_opcode: ");
    Serial.println(sfr::rockblock::f_opcode);
    Serial.print("f_arg_1: ");
    Serial.println(sfr::rockblock::f_arg_1);
    Serial.print("f_arg_2: ");
    Serial.println(sfr::rockblock::f_arg_2);

    Serial.println("----------------------IMU----------------------------");
    Serial.print("mag_x: ");
    Serial.println(sfr::imu::mag_x);
    Serial.print("mag_y: ");
    Serial.println(sfr::imu::mag_y);
    Serial.print("mag_z: ");
    Serial.println(sfr::imu::mag_z);
    Serial.print("gyro_x: ");
    Serial.println(sfr::imu::gyro_x);
    Serial.print("gyro_y: ");
    Serial.println(sfr::imu::gyro_y);
    Serial.print("gyro_z: ");
    Serial.println(sfr::imu::gyro_z);

    Serial.println("------------------TEMPERATURE-----------------------");
    Serial.print("temp_c: ");
    Serial.println(sfr::temperature::temp_c);
    Serial.print("mode: ");
    switch((int)sfr::temperature::mode){
        case 0:
            Serial.println("inactive");
            break;
        case 1:
            Serial.println("active");
            break;
    }

    Serial.println("--------------------CURRENT-----------------------");
    Serial.print("solar_current: ");
    Serial.println(sfr::current::solar_current);

    Serial.println("--------------------BATTERY-----------------------");
    Serial.print("voltage: ");
    Serial.println(sfr::battery::voltage);

    Serial.println("--------------------BUTTON-----------------------");
    Serial.print("pressed: ");
    Serial.println(sfr::button::pressed);
    #endif
    
    uint8_t mag_x = map(sfr::imu::mag_x_average, constants::imu::min_mag_x, constants::imu::max_mag_x, 0, 255);
    uint8_t mag_y = map(sfr::imu::mag_y_average, constants::imu::min_mag_y, constants::imu::max_mag_y, 0, 255);
    uint8_t mag_z = map(sfr::imu::mag_z_average, constants::imu::min_mag_z, constants::imu::max_mag_z, 0, 255);

    uint8_t gyro_x = map(sfr::imu::gyro_x_average, constants::imu::min_gyro_x, constants::imu::max_gyro_x, 0, 255);
    uint8_t gyro_y = map(sfr::imu::gyro_y_average, constants::imu::min_gyro_y, constants::imu::max_gyro_y, 0, 255);
    uint8_t gyro_z = map(sfr::imu::gyro_z_average, constants::imu::min_gyro_z, constants::imu::max_gyro_z, 0, 255);

    uint8_t temp_c = map(sfr::temperature::temp_c_average, constants::temperature::min_temp_c, constants::temperature::max_temp_c, 0, 255);

    uint8_t solar_current = map(sfr::current::solar_current_average, constants::current::min_solar_current, constants::current::max_solar_current, 0, 255);

    uint8_t voltage = map(sfr::battery::voltage_average, constants::battery::min_voltage, constants::battery::max_voltage, 0, 255);

    uint8_t downlink_period = map(sfr::rockblock::downlink_period, constants::rockblock::min_downlink_period, constants::rockblock::max_downlink_period, 0, 255);

    sfr::rockblock::report[0] = sfr::photoresistor::covered;
    sfr::rockblock::report[1] = sfr::button::pressed;
    sfr::rockblock::report[2] = (uint8_t)sfr::mission::mode;
    sfr::rockblock::report[3] = sfr::burnwire::fire;
    sfr::rockblock::report[4] = sfr::burnwire::arm;
    sfr::rockblock::report[5] = (uint8_t)sfr::burnwire::mode;
    sfr::rockblock::report[6] = sfr::burnwire::attempts;
    sfr::rockblock::report[7] = downlink_period;
    sfr::rockblock::report[8] = sfr::rockblock::waiting_message;
    sfr::rockblock::report[9] = sfr::rockblock::waiting_command;
    sfr::rockblock::report[10] = sfr::rockblock::f_opcode;
    sfr::rockblock::report[11] = sfr::rockblock::f_arg_1;
    sfr::rockblock::report[12] = sfr::rockblock::f_arg_2;
    sfr::rockblock::report[13] = mag_x;
    sfr::rockblock::report[14] = mag_y;
    sfr::rockblock::report[15] = mag_z;
    sfr::rockblock::report[16] = gyro_x;
    sfr::rockblock::report[17] = gyro_y;
    sfr::rockblock::report[18] = gyro_z;
    sfr::rockblock::report[19] = temp_c;
    sfr::rockblock::report[20] = (uint8_t)sfr::temperature::mode;
    sfr::rockblock::report[21] = solar_current;
    sfr::rockblock::report[22] = sfr::current::in_sun;
    sfr::rockblock::report[23] = (uint8_t)sfr::acs::mode;
    sfr::rockblock::report[24] = voltage;
    sfr::rockblock::report[25] = (uint8_t)sfr::fault::mode;
    sfr::rockblock::report[26] = sfr::fault::check_mag_x;
    sfr::rockblock::report[27] = sfr::fault::check_mag_y;
    sfr::rockblock::report[28] = sfr::fault::check_mag_z;
    sfr::rockblock::report[29] = sfr::fault::check_gyro_x;
    sfr::rockblock::report[30] = sfr::fault::check_gyro_y;
    sfr::rockblock::report[31] = sfr::fault::check_gyro_z;
    sfr::rockblock::report[32] = sfr::fault::check_temp_c;
    sfr::rockblock::report[33] = sfr::fault::check_voltage;
    sfr::rockblock::report[34] = sfr::fault::check_solar_current;
    sfr::rockblock::report[35] = sfr::camera::take_photo;
    sfr::rockblock::report[36] = sfr::camera::powered;
}

#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop()
    : battery_monitor(),
      button_monitor(),
      camera_report_monitor(),
      command_monitor(),
      current_monitor(),
      normal_report_monitor(),
      imu_monitor(),
      imudownlink_report_monitor(),
      imu_downlink(),
      photoresistor_monitor(),
      rockblock_report_monitor(),
      temperature_monitor(),
      burnwire_control_task(),
      camera_control_task(),
      rockblock_control_task(),
      eeprom_control_task(),
      mission_manager(),
      acs_control_task(),
      clock_manager()
{
    delay(1000);
}

void MainControlLoop::execute()
{
// #ifdef FRAGMENT
//     Serial.println("--------------------START LOOP--------------------");
//     // TODO print time on here

//     // mission mode
//     Serial.print("Current Mission Mode: ");
//     Serial.println(sfr::mission::current_mode->get_name().c_str());

//     // RockBLOCK
//     if (sfr::rockblock::sleep_mode) {
//         Serial.println("RockBLOCK sleeping");
//     } else {
//         Serial.println("RockBLOCK NOT sleeping");
//     }

//     // EEPROM
//     Serial.print("Total time alive (across all boots): ");
//     Serial.println(sfr::eeprom::time_alive);
// #endif

#ifdef VERBOSE
    Serial.println("--------------------START LOOP--------------------");

    // mission mode
    Serial.print("Current Mission Mode: ");
    Serial.println(sfr::mission::current_mode->get_name().c_str());

    // button
    if (sfr::button::pressed) {
        Serial.println("Button pressed");
    } else {
        Serial.println("Button UNpressed");
    }
    if (sfr::button::button_pressed->is_valid()) {
        Serial.println("Button valid");
    } else {
        Serial.println("Button INvalid");
    }

    // photoresistor
    if (sfr::photoresistor::covered) {
        Serial.println("Photoresistor covered");
    } else {
        Serial.println("Photoresistor UNcovered");
    }
    if (sfr::photoresistor::light_val_average_standby->is_valid()) {
        Serial.println("Photoresistor valid");
    } else {
        Serial.println("Photoresistor INvalid");
    }

    // ACS
    Serial.print("ACS Mode: ");
    switch (sfr::acs::mode) {
    case ((uint8_t)acs_mode_type::simple):
        Serial.println("SIMPLE");
        break;
    case ((uint8_t)acs_mode_type::point):
        Serial.println("POINT");
        break;
    case ((uint8_t)acs_mode_type::detumble):
        Serial.println("DETUMBLE");
        break;
    }

    if (sfr::acs::off) {
        Serial.println("ACS OFF");
    } else {
        Serial.println("ACS ON");
    }

    // battery
    if (sfr::battery::voltage_average->get_value(&val)) {
        Serial.print("Battery Voltage: ");
        Serial.print(val);
        Serial.println(" V");
    } else {
        Serial.println("Battery INvalid");
    }

    // IMU
    if (sfr::imu::powered) {
        Serial.println("IMU powered");
    } else {
        Serial.println("IMU UNpowered");
    }
    if (sfr::imu::mag_x_value->get_value(&val)) {
        Serial.print("MAG X: ");
        Serial.println(val);
    }
    if (sfr::imu::mag_y_value->get_value(&val)) {
        Serial.print("MAG Y: ");
        Serial.println(val);
    }
    if (sfr::imu::mag_z_value->get_value(&val)) {
        Serial.print("MAG Z: ");
        Serial.println(val);
    }
    if (sfr::imu::gyro_x_value->get_value(&val)) {
        Serial.print("GYRO X: ");
        Serial.println(val);
    }
    if (sfr::imu::gyro_y_value->get_value(&val)) {
        Serial.print("GYRO Y: ");
        Serial.println(val);
    }
    if (sfr::imu::gyro_z_value->get_value(&val)) {
        Serial.print("GYRO Z: ");
        Serial.println(val);
    }
    val = sfr::imu::gyro_x_value->get_value(&val) && sfr::imu::gyro_y_value->get_value(&val) && sfr::imu::gyro_z_value->get_value(&val) && sfr::imu::mag_x_value->get_value(&val) && sfr::imu::mag_y_value->get_value(&val) && sfr::imu::mag_z_value->get_value(&val);
    if (!val) {
        Serial.println("IMU INvalid");
    } else {
        Serial.println("IMU valid");
    }

    Serial.print("IMU initialization failed attempts: ");
    Serial.println(sfr::imu::failed_times);

    // Temp
    if (sfr::temperature::temp_c_average->get_value(&val)) {
        Serial.print("Temp: ");
        Serial.print(val);
        Serial.println(" C");
    } else {
        Serial.println("Temp INvalid");
    }

    // RockBLOCK
    if (sfr::rockblock::sleep_mode) {
        Serial.println("RockBLOCK sleeping");
    } else {
        Serial.println("RockBLOCK NOT sleeping");
    }

    Serial.print("RockBLOCK Mode: ");
    Serial.println(sfr::rockblock::mode.get());

    // EEPROM
    Serial.print("Total time alive (across all boots): ");
    Serial.println(sfr::eeprom::time_alive);

#endif
    sfr::mission::cycle_start = millis();

    mission_manager.execute();
    burnwire_control_task.execute();
    rockblock_control_task.execute();
    command_monitor.execute();

    camera_control_task.execute();
    acs_control_task.execute();

    imu_monitor.execute();
    battery_monitor.execute();
    button_monitor.execute();
    current_monitor.execute();
    photoresistor_monitor.execute();
    rockblock_report_monitor.execute();
    temperature_monitor.execute();

    imu_downlink.execute();

    normal_report_monitor.execute();
    camera_report_monitor.execute();
    imudownlink_report_monitor.execute();

    eeprom_control_task.execute();

    // Clock Manager MUST run last
    clock_manager.execute();

#ifdef VERBOSE
    Serial.print("Cycle time (ms): ");
    Serial.println(millis() - sfr::mission::cycle_start);
    Serial.println("--------------------END LOOP--------------------");
    Serial.println(" ");
#endif
}
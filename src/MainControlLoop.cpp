#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop()
    : ControlTask<void>(),
      clock_manager(constants::timecontrol::control_cycle_time),
      battery_monitor(constants::timecontrol::battery_monitor_offset),
      button_monitor(constants::timecontrol::button_monitor_offset),
      camera_report_monitor(constants::timecontrol::camera_report_monitor_offset),
      command_monitor(constants::timecontrol::command_monitor_offset),
      current_monitor(constants::timecontrol::current_monitor_offset),
      normal_report_monitor(constants::timecontrol::normal_report_monitor_offset),
      imu_monitor(constants::timecontrol::imu_monitor_offset),
      imudownlink_report_monitor(constants::timecontrol::imudownlink_report_monitor_offset),
      imu_downlink(constants::timecontrol::imudownlink_report_monitor_offset),
      photoresistor_monitor(constants::timecontrol::photoresistor_monitor_offset),
      rockblock_report_monitor(constants::timecontrol::rockblock_report_monitor_offset),
      temperature_monitor(constants::timecontrol::temperature_monitor_offset),
      burnwire_control_task(constants::timecontrol::burnwire_control_task_offset),
      camera_control_task(constants::timecontrol::camera_control_task_offset),
      rockblock_control_task(constants::timecontrol::rockblock_control_task_offset),
      eeprom_control_task(constants::timecontrol::eeprom_control_task_offset),
      mission_manager(constants::timecontrol::mission_manager_offset),
      acs_control_task(constants::timecontrol::acs_monitor_offset)
{
    delay(1000);
}

void MainControlLoop::execute()
{
    delay(50);

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
    case (0):
        Serial.println("SIMPLE");
        break;
    case (1):
        Serial.println("POINT");
        break;
    case (2):
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
    }

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

#endif

    clock_manager.execute();
    burnwire_control_task.execute_on_time();
    rockblock_control_task.execute_on_time();
    command_monitor.execute_on_time();
    mission_manager.execute_on_time();

    battery_monitor.execute_on_time();
    button_monitor.execute_on_time();
    camera_report_monitor.execute_on_time();
    current_monitor.execute_on_time();
    imu_monitor.execute_on_time();
    imu_downlink.execute_on_time();
    imudownlink_report_monitor.execute_on_time();
    normal_report_monitor.execute_on_time();
    photoresistor_monitor.execute_on_time();
    rockblock_report_monitor.execute_on_time();
    temperature_monitor.execute_on_time();
    acs_control_task.execute_on_time();
    camera_control_task.execute_on_time();

    eeprom_control_task.execute_on_time();

#ifdef VERBOSE
    Serial.println("--------------------END LOOP--------------------");
    Serial.println(" ");
#endif
}
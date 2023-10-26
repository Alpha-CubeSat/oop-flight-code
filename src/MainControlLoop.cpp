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

    uint32_t start1 = micros();
    mission_manager.execute();
    uint32_t diff1 = micros() - start1;

    uint32_t start2 = micros();
    burnwire_control_task.execute();
    uint32_t diff2 = micros() - start2;

    uint32_t start3 = micros();
    rockblock_control_task.execute();
    uint32_t diff3 = micros() - start3;

    uint32_t start4 = micros();
    command_monitor.execute();
    uint32_t diff4 = micros() - start4;

    uint32_t start5 = micros();
    camera_control_task.execute();
    uint32_t diff5 = micros() - start5;

    uint32_t start6 = micros();
    acs_control_task.execute();
    uint32_t diff6 = micros() - start6;

    uint32_t start7 = micros();
    imu_monitor.execute();
    uint32_t diff7 = micros() - start7;

    uint32_t start8 = micros();
    battery_monitor.execute();
    uint32_t diff8 = micros() - start8;

    uint32_t start9 = micros();
    button_monitor.execute();
    uint32_t diff9 = micros() - start9;

    uint32_t start10 = micros();
    current_monitor.execute();
    uint32_t diff10 = micros() - start10;

    uint32_t start11 = micros();
    photoresistor_monitor.execute();
    uint32_t diff11 = micros() - start11;

    uint32_t start12 = micros();
    rockblock_report_monitor.execute();
    uint32_t diff12 = micros() - start12;

    uint32_t start13 = micros();
    temperature_monitor.execute();
    uint32_t diff13 = micros() - start13;

    uint32_t start14 = micros();
    imu_downlink.execute();
    uint32_t diff14 = micros() - start14;

    uint32_t start15 = micros();
    normal_report_monitor.execute();
    uint32_t diff15 = micros() - start15;

    uint32_t start16 = micros();
    camera_report_monitor.execute();
    uint32_t diff16 = micros() - start16;

    uint32_t start17 = micros();
    imudownlink_report_monitor.execute();
    uint32_t diff17 = micros() - start17;

    uint32_t start18 = micros();
    eeprom_control_task.execute();
    uint32_t diff18 = micros() - start18;

    // Clock Manager MUST run last
    clock_manager.execute();

    #ifdef VERBOSE_TIMINGS
    Serial.println("START");
    Serial.print("Current Mission Mode: ");
    Serial.println(sfr::mission::current_mode->get_name().c_str());
    Serial.println("Timings for each execution (us):");
    Serial.println(diff1);
    Serial.println(diff2);
    Serial.println(diff3);
    Serial.println(diff4);
    Serial.println(diff5);
    Serial.println(diff6);
    Serial.println(diff7);
    Serial.println(diff8);
    Serial.println(diff9);
    Serial.println(diff10);
    Serial.println(diff11);
    Serial.println(diff12);
    Serial.println(diff13);
    Serial.println(diff14);
    Serial.println(diff15);
    Serial.println(diff16);
    Serial.println(diff17);
    Serial.println(diff18);
    Serial.print("Cycle time (ms): ");
    Serial.println(millis() - sfr::mission::cycle_start);
    Serial.println("END");
#endif

#ifdef VERBOSE
    Serial.print("Cycle time (ms): ");
    Serial.println(millis() - sfr::mission::cycle_start);
    Serial.println("--------------------END LOOP--------------------");
    Serial.println(" ");
#endif
}
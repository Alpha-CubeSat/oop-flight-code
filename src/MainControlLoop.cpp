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
      acs_control_task()
{
    delay(1000);
    last_millis = 0;
}

void MainControlLoop::execute()
{

#ifdef VERBOSE
    Serial.println("--------------------START LOOP--------------------");

    // Serial cycle time elapsed, place after clock manager for consistency
    last_millis = millis();

    // mission mode
    Serial.print("Current Mission Mode: ");
    Serial.println(sfr::mission::current_mode->get_name().c_str());

    sfr::temperature::in_sun = true;

    Serial.println(sfr::rockblock::mode.get());
#endif

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

    cycle_time = millis() - last_millis;

    if(cycle_time < 100){
        delay(100 - cycle_time);
    }

#ifdef VERBOSE
    Serial.print("Cycle time (ms): ");
    Serial.println((millis() - last_millis));
    Serial.println("--------------------END LOOP--------------------");
    Serial.println(" ");
#endif
}
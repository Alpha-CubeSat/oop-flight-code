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
    (void)Watchdog.enable(constants::watchdog::max_period_ms);
}

void MainControlLoop::execute()
{
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

    // Pet watchdog

    Watchdog.reset();

#ifdef VERBOSE
    Serial.print("Cycle time (ms): ");
    Serial.println(millis() - sfr::mission::cycle_start);
    Serial.println("--------------------END LOOP--------------------");
    Serial.println(" ");
#endif
}
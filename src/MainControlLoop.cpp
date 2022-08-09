#include "MainControlLoop.hpp"

int start;
int test_count = 0;

MainControlLoop::MainControlLoop()
    : ControlTask<void>(),
      clock_manager(constants::timecontrol::control_cycle_time),
      acs_monitor(constants::timecontrol::acs_monitor_offset),
      battery_monitor(constants::timecontrol::battery_monitor_offset),
      button_monitor(constants::timecontrol::button_monitor_offset),
      camera_report_monitor(constants::timecontrol::camera_report_monitor_offset),
      command_monitor(constants::timecontrol::command_monitor_offset),
      current_monitor(constants::timecontrol::current_monitor_offset),
      fault_monitor(constants::timecontrol::fault_monitor_offset),
      normal_report_monitor(constants::timecontrol::normal_report_monitor_offset),
      imu_monitor(constants::timecontrol::imu_monitor_offset),
      imudownlink_report_monitor(constants::timecontrol::imudownlink_report_monitor_offset),
      imu_downlink(constants::timecontrol::imudownlink_report_monitor_offset),
      photoresistor_monitor(constants::timecontrol::photoresistor_monitor_offset),
      rockblock_report_monitor(constants::timecontrol::rockblock_report_monitor_offset),
      temperature_monitor(constants::timecontrol::temperature_monitor_offset),
      acs_control_task(constants::timecontrol::acs_control_task_offset),
      burnwire_control_task(constants::timecontrol::burnwire_control_task_offset),
      camera_control_task(constants::timecontrol::camera_control_task_offset),
      rockblock_control_task(constants::timecontrol::rockblock_control_task_offset),
      eeprom_control_task(),
      mission_manager(constants::timecontrol::mission_manager_offset)

{
    delay(1000);
}

void MainControlLoop::execute()
{
    delay(200);
    faults::fault_1 = 0;
    faults::fault_2 = 0;
    faults::fault_3 = 0;

    clock_manager.execute();

    mission_manager.execute_on_time();

    acs_monitor.execute_on_time();
    battery_monitor.execute_on_time();

    button_monitor.execute_on_time();
    camera_report_monitor.execute_on_time();
    command_monitor.execute_on_time();
    current_monitor.execute_on_time();
    fault_monitor.execute_on_time();
    imu_monitor.execute_on_time();
    imu_downlink.execute_on_time();
    imudownlink_report_monitor.execute_on_time();
    normal_report_monitor.execute_on_time();
    photoresistor_monitor.execute_on_time();
    rockblock_report_monitor.execute_on_time();
    temperature_monitor.execute_on_time();
    acs_control_task.execute_on_time();
    burnwire_control_task.execute_on_time();
    camera_control_task.execute_on_time();
    rockblock_control_task.execute_on_time();
    eeprom_control_task.execute();

    sfr::imu::turn_on = true;

    Serial.printf("End of the loop\n\n");
}

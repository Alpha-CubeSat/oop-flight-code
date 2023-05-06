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
      mission_manager(constants::timecontrol::mission_manager_offset)
{
    delay(1000);
}

#ifdef E2E_TESTING
bool first_iter = true;
#endif

void MainControlLoop::execute()
{
    delay(200); // To prolong the speed of the main control loop to ensure correct RockBlock reads. Can reduce in the future.

    faults::fault_1 = 0;
    faults::fault_2 = 0;
    faults::fault_3 = 0;

#ifdef E2E_TESTING
    if (first_iter) {
        first_iter = false;
        sfr::mission::current_mode = sfr::mission::normal;
    }
#endif
    clock_manager.execute();
    mission_manager.execute_on_time();
    battery_monitor.execute_on_time();
    button_monitor.execute_on_time();
    camera_report_monitor.execute_on_time();
    command_monitor.execute_on_time();
    current_monitor.execute_on_time();
    imu_monitor.execute_on_time();
    imu_downlink.execute_on_time();
    imudownlink_report_monitor.execute_on_time();
    normal_report_monitor.execute_on_time();
    photoresistor_monitor.execute_on_time();
    rockblock_report_monitor.execute_on_time();
    temperature_monitor.execute_on_time();
    burnwire_control_task.execute_on_time();
    camera_control_task.execute_on_time();
    rockblock_control_task.execute_on_time();
    eeprom_control_task.execute_on_time();
}
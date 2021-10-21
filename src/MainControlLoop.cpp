#include "MainControlLoop.hpp"

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
imu_monitor(constants::timecontrol::imu_monitor_offset),
normal_report_monitor(constants::timecontrol::normal_report_monitor_offset),
photoresistor_monitor(constants::timecontrol::photoresistor_monitor_offset),
temperature_monitor(constants::timecontrol::temperature_monitor_offset),
acs_control_task(constants::timecontrol::acs_control_task_offset),
burnwire_control_task(constants::timecontrol::burnwire_control_task_offset),
camera_control_task(constants::timecontrol::camera_control_task_offset),
rockblock_control_task(constants::timecontrol::rockblock_control_task_offset),
temperature_control_task(constants::timecontrol::temperature_control_task_offset),
mission_manager(constants::timecontrol::mission_manager_offset)
{
    delay(1000);
    MissionManager::transition_to_initialization();    
}

void MainControlLoop::execute()
{
    sfr::fault::fault_1 = 0;
    sfr::fault::fault_2 = 0;
    sfr::fault::fault_3 = 0;

    clock_manager.execute();

    MissionManager::transition_to_deployment();  

    //acs_monitor.execute_on_time();
    battery_monitor.execute_on_time();
    button_monitor.execute_on_time();
    camera_report_monitor.execute_on_time();
    command_monitor.execute_on_time();
    current_monitor.execute_on_time();
    fault_monitor.execute_on_time();
    imu_monitor.execute_on_time();
    normal_report_monitor.execute_on_time();
    photoresistor_monitor.execute_on_time();
    temperature_monitor.execute_on_time();

    //acs_control_task.execute_on_time();
    burnwire_control_task.execute_on_time();
    camera_control_task.execute_on_time();
    rockblock_control_task.execute_on_time();
    temperature_control_task.execute_on_time();

    mission_manager.execute_on_time();

    Serial.print("mag ");
    Serial.print(sfr::imu::mag_x_average);
    Serial.print(" ");
    Serial.print(sfr::imu::mag_y_average);
    Serial.print(" ");
    Serial.println(sfr::imu::mag_z_average);

    Serial.print("gyro ");
    Serial.print(sfr::imu::gyro_x_average);
    Serial.print(" ");
    Serial.print(sfr::imu::gyro_y_average);
    Serial.print(" ");
    Serial.println(sfr::imu::gyro_z_average);

    if(sfr::mission::mode == mission_mode_type::standby)
        Serial.println("standby");
    else if(sfr::mission::mode == mission_mode_type::safe)
        Serial.println("safe");
    else if(sfr::mission::mode == mission_mode_type::initialization)
        Serial.println("initialization");
    else if(sfr::mission::mode == mission_mode_type::low_power)
        Serial.println("low_power");
    else if(sfr::mission::mode == mission_mode_type::deployment)
        Serial.println("deployment");
}
#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop()
    : ControlTask<void>(), clock_manager(constants::timecontrol::control_cycle_time), acs_monitor(constants::timecontrol::acs_monitor_offset), battery_monitor(constants::timecontrol::battery_monitor_offset), button_monitor(constants::timecontrol::button_monitor_offset), camera_report_monitor(constants::timecontrol::camera_report_monitor_offset), command_monitor(constants::timecontrol::command_monitor_offset), current_monitor(constants::timecontrol::current_monitor_offset), fault_monitor(constants::timecontrol::fault_monitor_offset), imu_monitor(constants::timecontrol::imu_monitor_offset), normal_report_monitor(constants::timecontrol::normal_report_monitor_offset), photoresistor_monitor(constants::timecontrol::photoresistor_monitor_offset), rockblock_report_monitor(constants::timecontrol::rockblock_report_monitor_offset), temperature_monitor(constants::timecontrol::temperature_monitor_offset), acs_control_task(constants::timecontrol::acs_control_task_offset), burnwire_control_task(constants::timecontrol::burnwire_control_task_offset), camera_control_task(constants::timecontrol::camera_control_task_offset), rockblock_control_task(constants::timecontrol::rockblock_control_task_offset), mission_manager(constants::timecontrol::mission_manager_offset)
{
    delay(1000);
}

int count = 0;

void MainControlLoop::execute()
{
    delay(200);
    faults::fault_1 = 0;
    faults::fault_2 = 0;
    faults::fault_3 = 0;

    clock_manager.execute();

    // switching mission mode happens in mission manager
    mission_manager.execute_on_time();

    acs_monitor.execute_on_time();
    battery_monitor.execute_on_time();
   
    button_monitor.execute_on_time();
    camera_report_monitor.execute_on_time();
    command_monitor.execute_on_time();
    current_monitor.execute_on_time();
    fault_monitor.execute_on_time();
    imu_monitor.execute_on_time();
    normal_report_monitor.execute_on_time();
    photoresistor_monitor.execute_on_time();
    rockblock_report_monitor.execute_on_time();
    temperature_monitor.execute_on_time();

    acs_control_task.execute_on_time();
    burnwire_control_task.execute_on_time();
    camera_control_task.execute_on_time();
    rockblock_control_task.execute_on_time();

    /* Testing Transitions in Firing Phase */

    // // test from turn on camera to mandatory burns due to init mode is complete
    // if(count++ == 0) sfr::mission::current_mode = sfr::mission::bootCamera;
    // sfr::camera::init_mode = camera_init_mode_type::complete;
    // Serial.printf("current id : %d\n\n", sfr::mission::current_mode->get_id());

    // // test from turn on camera (18) to mandatory burns (19) due to failed times is larger than 5
    // if(count++ == 0) sfr::mission::current_mode = sfr::mission::bootCamera;
    // sfr::camera::turn_on = true;
    // sfr::camera::powered = false;
    // sfr::camera::init_mode = camera_init_mode_type::failed;
    // Serial.printf("failed times : %d\n", sfr::camera::failed_times);
    // Serial.printf("current id : %d\n\n", sfr::mission::current_mode->get_id());

    // // test from mandatory burns to regular burns 
    // if(count++ == 0) sfr::mission::current_mode = sfr::mission::mandatoryBurns;
    // Serial.printf("current id : %d\n\n", sfr::mission::current_mode->get_id());

    // // test from regular burns to take photo due to button unpressed 
    // if(count++ == 0) sfr::mission::current_mode = sfr::mission::regularBurns;
    // sfr::photoresistor::covered = true;
    // sfr::button::pressed = false;
    // Serial.printf("current id : %d\n\n", sfr::mission::current_mode->get_id());

    // // test from regular burns to take photo due to photoresistor uncovered
    // if(count++ == 0) sfr::mission::current_mode = sfr::mission::regularBurns;
    // sfr::button::pressed = true;
    // sfr::photoresistor::covered = false;
    // Serial.printf("current id : %d\n\n", sfr::mission::current_mode->get_id());

    // // test from regular burns to armed transition due to button pressed after X burns
    // // zp74, BurnwireControlTask::dispatch_burn method isn't called, can't test attempts, still in development?
    // if(count++ == 0) sfr::mission::current_mode = sfr::mission::regularBurns;
    // sfr::button::pressed = true;
    // sfr::photoresistor::covered = true;
    // sfr::burnwire::attempts = 11;
    // Serial.printf("current id : %d\n\n", sfr::mission::current_mode->get_id());

    // test exit in sun phase
    if(count++ == 0) sfr::mission::current_mode = sfr::mission::normalInSun;
    // sfr::temperature::temp_c_average->set_valid();
    // sfr::temperature::in_sun = true;

    // sfr::temperature::temp_c_average->set_invalid();
    // sfr::current::solar_current_average->set_valid();
    // sfr::current::in_sun = true;
    Serial.printf("current id : %d\n\n", sfr::mission::current_mode->get_id());

}

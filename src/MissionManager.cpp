#include "MissionManager.hpp"

MissionManager::MissionManager(unsigned int offset) : TimedControlTask<void>(offset)
{
    transition_to_standby();
}

void MissionManager::execute()
{
    mission_mode_type mode = sfr::mission::mode;

    switch (mode) {
    case mission_mode_type::initialization:
        dispatch_initialization();
        break;
    case mission_mode_type::low_power:
        dispatch_low_power();
        break;
    case mission_mode_type::safe:
        dispatch_safe();
        break;
    case mission_mode_type::standby:
        dispatch_standby();
        break;
    case mission_mode_type::deployment:
        dispatch_deployment();
        break;
    }
}

void MissionManager::dispatch_initialization()
{
    if (sfr::rockblock::num_downlinks >= constants::rockblock::num_initial_downlinks) {
        transition_to_standby();
    }
}

void MissionManager::dispatch_standby()
{
    if (sfr::battery::voltage < 3.75 && sfr::fault::check_voltage && sfr::mission::low_power_eligible == true) {
        transition_to_low_power();
    }
}

void MissionManager::dispatch_safe() {}

void MissionManager::dispatch_low_power()
{
    if (sfr::battery::voltage > 3.9 and sfr::fault::check_voltage && sfr::mission::low_power_eligible == true) {
        transition_to_standby();
    }
}

void MissionManager::dispatch_deployment()
{
    if (sfr::battery::voltage < 3.75 && sfr::fault::check_voltage) {
        transition_to_low_power();
    }
    if (!sfr::button::pressed && !sfr::photoresistor::covered) {
        sfr::camera::take_photo = true;
        BurnwireControlTask::transition_to_standby();
        transition_to_standby();
    }
}

void MissionManager::transition_to_initialization()
{
    sfr::mission::mode = mission_mode_type::initialization;
    sfr::fault::mode = fault_mode_type::active;
    sfr::acs::mode = acs_mode_type::simple;
    sfr::rockblock::downlink_period = constants::rockblock::ten_minutes;
}

void MissionManager::transition_to_standby()
{
    sfr::mission::mode = mission_mode_type::standby;
    sfr::fault::mode = fault_mode_type::active;
    sfr::acs::mode = acs_mode_type::full;
    sfr::rockblock::downlink_period = constants::rockblock::ten_minutes;
}

void MissionManager::transition_to_safe()
{
    sfr::mission::mode = mission_mode_type::safe;
    sfr::acs::mode = acs_mode_type::simple;
    sfr::rockblock::downlink_period = constants::rockblock::two_hours;
    sfr::fault::mode = fault_mode_type::inactive;
}

void MissionManager::transition_to_low_power()
{
    sfr::mission::mode = mission_mode_type::low_power;
    sfr::fault::mode = fault_mode_type::active;
    sfr::acs::mode = acs_mode_type::off;
    sfr::rockblock::downlink_period = constants::rockblock::two_hours;
}

void MissionManager::transition_to_deployment()
{
    sfr::camera::turn_off = true;
    sfr::mission::mode = mission_mode_type::deployment;
    sfr::acs::mode = acs_mode_type::simple;
    sfr::rockblock::downlink_period = constants::rockblock::ten_minutes;
}

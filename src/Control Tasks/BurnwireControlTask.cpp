#include "BurnwireControlTask.hpp"

BurnwireControlTask::BurnwireControlTask(unsigned int offset) : TimedControlTask<void>(offset) {}

void BurnwireControlTask::execute()
{
    burnwire_mode_type mode = sfr::burnwire::mode;

    if (mode == burnwire_mode_type::delay || mode == burnwire_mode_type::burn) {
        sfr::mission::low_power_eligible = false;
    } else {
        sfr::mission::low_power_eligible = true;
    }

    switch (mode) {
    case burnwire_mode_type::standby: {
        if (sfr::burnwire::arm && sfr::mission::mode == mission_mode_type::mand_burns) {
            sfr::burnwire::mode = burnwire_mode_type::armed;
            sfr::burnwire::start_time = millis();
            sfr::burnwire::fire = false;
        } else {
            transition_to_standby();
        }
        break;
    }
    case burnwire_mode_type::armed: {
        if (sfr::mission::mode == mission_mode_type::mand_burns) {
            if (millis() - sfr::burnwire::start_time >= (uint32_t)sfr::burnwire::armed_time) {
                transition_to_standby();
            }
            if ((sfr::burnwire::fire && sfr::temperature::in_sun && sfr::fault::check_temp_c) ||
                (sfr::burnwire::fire && !sfr::fault::check_temp_c && sfr::fault::check_solar_current && sfr::current::in_sun) ||
                (sfr::burnwire::fire && !sfr::fault::check_solar_current && !sfr::fault::check_temp_c)) {
                sfr::burnwire::mode = burnwire_mode_type::fire;
                sfr::camera::turn_on = true;
            } else {
                Pins::setPinState(constants::burnwire::first_pin, LOW);
                Pins::setPinState(constants::burnwire::second_pin, LOW);
            }
        } else {
            transition_to_standby();
        }
        break;
    }
    case burnwire_mode_type::fire: {
        if (sfr::mission::mode == mission_mode_type::mand_burns) {
            if (sfr::camera::powered) {
                dispatch_burn();
            } else if (sfr::burnwire::camera_attempts >= constants::burnwire::camera_max_attempts) {
                dispatch_burn();
            } else {
                sfr::burnwire::camera_attempts++;
            }
        } else {
            transition_to_standby();
        }
        break;
    }
    case burnwire_mode_type::burn: {
        if (sfr::mission::mode == mission_mode_type::mand_burns) {
            if (millis() - sfr::burnwire::start_time >= (uint32_t)sfr::burnwire::burn_time) {
                sfr::burnwire::mode = burnwire_mode_type::delay;
                Pins::setPinState(constants::burnwire::first_pin, LOW);
                Pins::setPinState(constants::burnwire::second_pin, LOW);
                sfr::burnwire::start_time = millis();
            } else {
                dispatch_burn();
            }
        } else {
            transition_to_standby();
        }
        break;
    }
    case burnwire_mode_type::delay: {
        if (sfr::mission::mode == mission_mode_type::mand_burns) {
            if (millis() - sfr::burnwire::start_time >= constants::burnwire::burn_wait) {
                dispatch_burn();
            } else {
                Pins::setPinState(constants::burnwire::first_pin, LOW);
                Pins::setPinState(constants::burnwire::second_pin, LOW);
            }
        } else {
            transition_to_standby();
        }
        break;
    }
    }
}

void BurnwireControlTask::dispatch_burn()
{
    sfr::burnwire::camera_attempts = 0;
    sfr::burnwire::mode = burnwire_mode_type::burn;
    sfr::burnwire::start_time = millis();
    if (sfr::burnwire::attempts > constants::burnwire::max_attempts) {
        transition_to_standby();
        // sfr::fault::fault_actions = sfr::fault::fault_actions | constants::fault::burn_wire;
    } else {
        if (sfr::burnwire::attempts % 2 == 0) {
            Pins::setPinState(constants::burnwire::first_pin, HIGH);
            Pins::setPinState(constants::burnwire::second_pin, LOW);
        } else {
            Pins::setPinState(constants::burnwire::first_pin, LOW);
            Pins::setPinState(constants::burnwire::second_pin, HIGH);
        }
        sfr::burnwire::attempts++;
    }
}

void BurnwireControlTask::transition_to_standby()
{
    sfr::burnwire::mode = burnwire_mode_type::standby;
    Pins::setPinState(constants::burnwire::first_pin, LOW);
    Pins::setPinState(constants::burnwire::second_pin, LOW);
    sfr::burnwire::fire = false;
    sfr::burnwire::arm = false;
    sfr::burnwire::attempts = 0;
}
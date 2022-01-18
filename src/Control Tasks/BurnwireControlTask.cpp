#include "BurnwireControlTask.hpp"

BurnwireControlTask::BurnwireControlTask(unsigned int offset) : TimedControlTask<void>(offset) {}

void BurnwireControlTask::execute()
{
}

void BurnwireControlTask::dispatch_burn()
{
    sfr::burnwire::camera_attempts = 0;
    sfr::burnwire::mode = burnwire_mode_type::burn;
    sfr::burnwire::start_time = millis();
    if (sfr::burnwire::attempts > constants::burnwire::max_attempts) {
        transition_to_standby();
        sfr::fault::fault_3 = sfr::fault::fault_3 | constants::fault::burn_wire;
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
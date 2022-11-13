#include "BurnwireControlTask.hpp"

BurnwireControlTask::BurnwireControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void BurnwireControlTask::execute()
{
    burnwire_mode_type mode = (burnwire_mode_type)sfr::burnwire::mode.get();

    switch (mode) {
    case burnwire_mode_type::standby: {
        if (sfr::mission::current_mode->get_type() == mode_type::BURN) {
            dispatch_burn();
        } else {
            transition_to_standby();
        }
        break;
    }
    case burnwire_mode_type::burn: {
        if (sfr::mission::current_mode->get_type() == mode_type::BURN) {
            if (millis() - sfr::burnwire::start_time >= (uint32_t)sfr::burnwire::burn_time) {
                sfr::burnwire::mode = (uint16_t)burnwire_mode_type::delay;
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
        if (sfr::mission::current_mode->get_type() == mode_type::BURN) {
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
    sfr::burnwire::mode = (uint16_t)burnwire_mode_type::burn;
    sfr::burnwire::start_time = millis();

    if (sfr::burnwire::attempts % 2 == 0) {
        Pins::setPinState(constants::burnwire::first_pin, HIGH);
        Pins::setPinState(constants::burnwire::second_pin, LOW);
    } else {
        Pins::setPinState(constants::burnwire::first_pin, LOW);
        Pins::setPinState(constants::burnwire::second_pin, HIGH);
    }
    sfr::burnwire::attempts++;
}

void BurnwireControlTask::transition_to_standby()
{
    sfr::burnwire::mode = (uint16_t)burnwire_mode_type::standby;
    Pins::setPinState(constants::burnwire::first_pin, LOW);
    Pins::setPinState(constants::burnwire::second_pin, LOW);
    sfr::burnwire::attempts = 0;
}

#include "BurnwireControlTask.hpp"

BurnwireControlTask::BurnwireControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void BurnwireControlTask::execute()
{
#ifdef BURNWIRE
    Serial.print("BURNWIRE: attempts ");
    Serial.println(sfr::burnwire::attempts);

    Serial.print("BURNWIRE: photoresistor covered ");
    Serial.println(sfr::photoresistor::covered);

    Serial.print("BURNWIRE: button pressed ");
    Serial.println(sfr::button::pressed);
#endif

    burnwire_mode_type mode = (burnwire_mode_type)sfr::burnwire::mode.get();

    switch (mode) {
    case burnwire_mode_type::standby: {
#ifdef BURNWIRE
        Serial.println("BURNWIRE: standby");
#endif
        if (sfr::mission::current_mode->get_type() == mode_type::BURN) {
            dispatch_burn();
        } else {
            transition_to_standby();
        }
        break;
    }
    case burnwire_mode_type::burn: {
#ifdef BURNWIRE
        Serial.println("BURNWIRE: burn");
#endif
        if (sfr::mission::current_mode->get_type() == mode_type::BURN) {
            if (millis() - sfr::burnwire::start_time >= sfr::burnwire::burn_time) {
                sfr::burnwire::mode = (uint16_t)burnwire_mode_type::delay;
                Pins::setPinState(constants::burnwire::first_pin, LOW);
                Pins::setPinState(constants::burnwire::second_pin, LOW);
                sfr::burnwire::start_time = millis();
            }
        } else {
            transition_to_standby();
        }
        break;
    }
    case burnwire_mode_type::delay: {
#ifdef BURNWIRE
        Serial.println("BURNWIRE: delay");
#endif
        if (sfr::mission::current_mode->get_type() == mode_type::BURN) {
            if (millis() - sfr::burnwire::start_time >= sfr::burnwire::delay_time) {
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
#ifdef BURNWIRE
        Serial.println("BURNWIRE: wire 1");
#endif
        Pins::setPinState(constants::burnwire::first_pin, HIGH);
        Pins::setPinState(constants::burnwire::second_pin, LOW);
    } else {
#ifdef BURNWIRE
        Serial.println("BURNWIRE: wire 2");
#endif
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

#include "ACSControlTask.hpp"

ACSControlTask::ACSControlTask(unsigned int offset) : TimedControlTask<void>(offset)
{}

void ACSControlTask::execute()
{
    /*ACSWrite(constants::acs::xtorqorder, sfr::acs::current1, constants::acs::xout1, constants::acs::xout2, constants::acs::xPWMpin);
    ACSWrite(constants::acs::ytorqorder, sfr::acs::current2, constants::acs::yout1, constants::acs::yout2, constants::acs::yPWMpin);
    ACSWrite(constants::acs::ztorqorder, sfr::acs::current3, constants::acs::zout1, constants::acs::zout2, constants::acs::zPWMpin);*/
}

void ACSControlTask::ACSWrite(int torqorder, int current, int out1, int out2, int PWMpin)
{
    if (current == 0.0)
    {
        Pins::setPinState(out1, LOW);
        Pins::setPinState(out2, LOW);
    }
    if (torqorder == 0)
    {
        if (current > 0)
        {
            Pins::setPinState(out1, HIGH);
            Pins::setPinState(out2, LOW);
            analogWrite(PWMpin, current);
        }
        if (current < 0)
        {
            Pins::setPinState(out1, LOW);
            Pins::setPinState(out2, HIGH);
            analogWrite(PWMpin, current);
        }
    }
    if (torqorder == 1)
    {
        if (current > 0)
        {
            Pins::setPinState(out1, LOW);
            Pins::setPinState(out2, HIGH);
            analogWrite(PWMpin, current);
        }
        if (current < 0)
        {
            Pins::setPinState(out1, HIGH);
            Pins::setPinState(out2, LOW);
            analogWrite(PWMpin, current);
        }
    }
}
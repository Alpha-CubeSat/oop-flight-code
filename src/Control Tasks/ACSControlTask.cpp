#include "ACSControlTask.hpp"

ACSControlTask::ACSControlTask(unsigned int offset) : TimedControlTask<void>(offset)
{
}

void ACSControlTask::execute()
{
    ACSWrite(constants::acs::xtorqorder, sfr::acs::current1, constants::acs::xout1, constants::acs::xout2, constants::acs::xPWMpin);
    ACSWrite(constants::acs::ytorqorder, sfr::acs::current2, constants::acs::yout1, constants::acs::yout2, constants::acs::yPWMpin);
    ACSWrite(constants::acs::ztorqorder, sfr::acs::current3, constants::acs::zout1, constants::acs::zout2, constants::acs::zPWMpin);
}

int ACSControlTask::current2PWM(float current)
{
    if (int(633.5 * pow(current, 0.6043) + 8.062) < 0)
        return 0;
    else if (int(633.5 * pow(current, 0.6043) + 8.062) > 255)
        return 255;
    else
        return int(633.5 * pow(current, 0.6043) + 8.062);
}

void ACSControlTask::ACSWrite(int torqorder, float current, int out1, int out2, int PWMpin)
{
    int PWM = current2PWM(abs(current));

    if (PWM == 0) {
        digitalWrite(out1, LOW);
        digitalWrite(out2, LOW);
    } else if (torqorder == 0) {
        if (current > 0) {
            digitalWrite(out1, HIGH);
            digitalWrite(out2, LOW);
            analogWrite(PWMpin, PWM);
        }
        if (current < 0) {
            digitalWrite(out1, LOW);
            digitalWrite(out2, HIGH);
            analogWrite(PWMpin, PWM);
        }
    } else if (torqorder == 1) {
        if (current > 0) {
            digitalWrite(out1, LOW);
            digitalWrite(out2, HIGH);
            analogWrite(PWMpin, PWM);
        }
        if (current < 0) {
            digitalWrite(out1, HIGH);
            digitalWrite(out2, LOW);
            analogWrite(PWMpin, PWM);
        }
    }
}
#include "ACSControlTask.hpp"

ACSControlTask::ACSControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void ACSControlTask::execute()
{
    ACSWrite(constants::acs::xtorqorder, sfr::acs::current1, constants::acs::xout1, constants::acs::xout2, constants::acs::xPWMpin);
    ACSWrite(constants::acs::ytorqorder, sfr::acs::current2, constants::acs::yout1, constants::acs::yout2, constants::acs::yPWMpin);
    ACSWrite(constants::acs::ztorqorder, sfr::acs::current3, constants::acs::zout1, constants::acs::zout2, constants::acs::zPWMpin);
    IMUOffset(sfr::temperature::temp_c, sfr::battery::voltage, sfr::acs::pwm1, sfr::acs::pwm2, sfr::acs::pwm3);
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

void ACSControlTask::IMUOffset(float temp, float voltage, float pwm1, float pwm2, float pwm3)
{
    /*

    xoffset = temp_x offset * voltage_x offset * (PWM_x offset + PWM_y offset + PWM_z offset)
    yoffset = temp_y offset * voltage_y offset * (PWM_x offset + PWM_y offset + PWM_z offset)
    zoffset = temp_z offset * voltage_z offset * (PWM_x offset + PWM_y offset + PWM_z offset)

    new mag_x += xoffset
    new mag_y += xoffset
    new mag_z += xoffset

    */
}

void ACSControlTask::ACSWrite(int torqorder, float current, int out1, int out2, int PWMpin)
{
    int PWM = current2PWM(abs(current));
    if (PWMpin == constants::acs::xPWMpin) {
        sfr::acs::pwm1 = PWM;
    } else if (PWMpin == constants::acs::yPWMpin) {
        sfr::acs::pwm2 = PWM;
    } else if (PWMpin == constants::acs::zPWMpin) {
        sfr::acs::pwm3 = PWM;
    }

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
#include "ACSControlTask.hpp"

ACSControlTask::ACSControlTask()
{
}

void ACSControlTask::execute()
{
    if ((old_Id != constants::acs::Id_values[sfr::acs::Id_index] || old_Kd != constants::acs::Kd_values[sfr::acs::Kd_index] || old_Kp != constants::acs::Kp_values[sfr::acs::Kp_index] || old_c != constants::acs::c_values[sfr::acs::c_index]) || first) {

#ifdef VERBOSE
        Serial.println("Initialize starshot library");
#endif
        starshotObj.initialize(constants::acs::step_size_input, constants::acs::A_input, constants::acs::Id_values[sfr::acs::Id_index], constants::acs::Kd_values[sfr::acs::Kd_index], constants::acs::Kp_values[sfr::acs::Kp_index], constants::acs::c_values[sfr::acs::c_index], constants::acs::i_max_input, constants::acs::k_input, constants::acs::n_input, constants::acs::target_spin_rate);

        first = false;
    }

    old_Id = constants::acs::Id_values[sfr::acs::Id_index];
    old_Kd = constants::acs::Kd_values[sfr::acs::Kd_index];
    old_Kp = constants::acs::Kp_values[sfr::acs::Kp_index];
    old_c = constants::acs::c_values[sfr::acs::c_index];

    imu_valid = sfr::imu::gyro_x_value->get_value(&gyro_x) &&
                sfr::imu::gyro_y_value->get_value(&gyro_y) &&
                sfr::imu::gyro_z_value->get_value(&gyro_z) &&
                sfr::imu::mag_x_value->get_value(&mag_x) &&
                sfr::imu::mag_y_value->get_value(&mag_y) &&
                sfr::imu::mag_z_value->get_value(&mag_z);

    if (!sfr::acs::off) {
        if ((!imu_valid && (sfr::imu::mode == (uint16_t)sensor_mode_type::abnormal_init || sfr::imu::mode == (uint16_t)sensor_mode_type::normal)) || sfr::acs::mode == (uint8_t)acs_mode_type::simple) {
            sfr::acs::mode = (uint8_t)acs_mode_type::simple;
            //  consider IMU valid since it is not needed for simple mode
            imu_valid = true;
        }

        if (imu_valid) {

            // read sfr data into local variables
            if (!sfr::imu::mag_x_value->get_value(&mag_x)) {
                mag_x = 0;
            }

            if (!sfr::imu::mag_y_value->get_value(&mag_y)) {
                mag_y = 0;
            }

            if (!sfr::imu::mag_z_value->get_value(&mag_z)) {
                mag_z = 0;
            }

            if (!sfr::imu::gyro_x_value->get_value(&gyro_x)) {
                gyro_x = 0;
            }

            if (!sfr::imu::gyro_y_value->get_value(&gyro_y)) {
                gyro_y = 0;
            }

            if (!sfr::imu::gyro_z_value->get_value(&gyro_z)) {
                gyro_z = 0;
            }

            //  Pass output of ekf into starshot
            starshotObj.rtU.Bfield_body[0] = mag_x / 1000000.0;
            starshotObj.rtU.Bfield_body[1] = mag_y / 1000000.0;
            starshotObj.rtU.Bfield_body[2] = mag_z / 1000000.0;
            starshotObj.rtU.w[0] = gyro_x;
            starshotObj.rtU.w[1] = gyro_y;
            starshotObj.rtU.w[2] = gyro_z;

            starshotObj.step();

            // Complete the loop (set current values to output of starshot)
            if (sfr::acs::mode == (uint8_t)acs_mode_type::detumble) {
                sfr::acs::current_x = starshotObj.rtY.detumble[0];
                sfr::acs::current_y = starshotObj.rtY.detumble[1];
                sfr::acs::current_z = starshotObj.rtY.detumble[2];
            } else if (sfr::acs::mode == (uint8_t)acs_mode_type::simple) {
                sfr::acs::current_x = 0;
                sfr::acs::current_y = 0;
                sfr::acs::current_z = 0;
                if (sfr::acs::simple_mag == (uint8_t)mag_type::x) {
                    sfr::acs::current_x = sfr::acs::simple_current.get_float();
                } else if (sfr::acs::simple_mag == (uint8_t)mag_type::y) {
                    sfr::acs::current_y = sfr::acs::simple_current.get_float();
                } else if (sfr::acs::simple_mag == (uint8_t)mag_type::z) {
                    sfr::acs::current_z = sfr::acs::simple_current.get_float();
                }
            }
        }
    }

    if (sfr::acs::off || !imu_valid) {
        sfr::acs::current_x = 0;
        sfr::acs::current_y = 0;
        sfr::acs::current_z = 0;
    }

    // ACSWrite(constants::acs::xtorqorder, sfr::acs::current_x, constants::acs::xout1, constants::acs::xout2, constants::acs::xPWMpin);
    // ACSWrite(constants::acs::ytorqorder, sfr::acs::current_y, constants::acs::yout1, constants::acs::yout2, constants::acs::yPWMpin);
    // ACSWrite(constants::acs::ztorqorder, sfr::acs::current_z, constants::acs::zout1, constants::acs::zout2, constants::acs::zPWMpin);
    sfr::acs::counter++;
    float test_current[9] = {-0.20, -0.15,-0.1,-0.05,0,0.05,0.1,0.15,0.20};
    // float test_current[2] = {0, 0};
    int index = sfr::acs::counter % 9;

    Serial.print("current(A): ");
    Serial.print(test_current[index]);
    Serial.print(", ");

    ACSWrite(constants::acs::xtorqorder, test_current[index], constants::acs::xout1, constants::acs::xout2, constants::acs::xPWMpin);
    ACSWrite(constants::acs::ytorqorder, test_current[index], constants::acs::yout1, constants::acs::yout2, constants::acs::yPWMpin);
    ACSWrite(constants::acs::ztorqorder, test_current[index], constants::acs::zout1, constants::acs::zout2, constants::acs::zPWMpin);

    // int PWM[9] = {-255,-236,-188,-115,14, 115, 188, 236, 255};
    // int index = sfr::acs::counter % 9;
    // ACSWrite_PWM(constants::acs::xtorqorder, PWM[index], constants::acs::xout1, constants::acs::xout2, constants::acs::xPWMpin);
    // ACSWrite_PWM(constants::acs::ytorqorder, PWM[index], constants::acs::yout1, constants::acs::yout2, constants::acs::yPWMpin);
    // ACSWrite_PWM(constants::acs::ztorqorder, PWM[index], constants::acs::zout1, constants::acs::zout2, constants::acs::zPWMpin);
    // Serial.print("PWM: ");
    // Serial.print(PWM[index]);
    // Serial.print(", ");

    // ACSWrite_PWM(constants::acs::xtorqorder, 0, constants::acs::xout1, constants::acs::xout2, constants::acs::xPWMpin);
    // ACSWrite_PWM(constants::acs::ytorqorder, 0, constants::acs::yout1, constants::acs::yout2, constants::acs::yPWMpin);
    // ACSWrite_PWM(constants::acs::ztorqorder, 0, constants::acs::zout1, constants::acs::zout2, constants::acs::zPWMpin);

#ifdef ACS_DATA
    Serial.print(millis());
    Serial.print(", ");
    Serial.print(sfr::mission::current_mode->get_name().c_str());
    Serial.print(", ");
    if (sfr::acs::off == false) {
        Serial.print("ON");
    } else {
        Serial.print("OFF");
    }
    Serial.print(", ");
    if (sfr::acs::mode == 0) {
        Serial.print("SIMPLE, ");
    } else if (sfr::acs::mode == 1) {
        Serial.print("DETUMBLE, ");
    }

    Serial.print(starshotObj.rtY.pt_error); // deg
    Serial.print(", ");
    Serial.print(sfr::acs::current_x);
    Serial.print(", ");
    Serial.print(sfr::acs::current_y);
    Serial.print(", ");
    Serial.print(sfr::acs::current_z);
    Serial.print(", ");
    Serial.print(mag_x); // uT
    Serial.print(", ");
    Serial.print(mag_y); // uT
    Serial.print(", ");
    Serial.print(mag_z); // uT
    Serial.print(", ");
    Serial.print(gyro_x);
    Serial.print(", ");
    Serial.print(gyro_y);
    Serial.print(", ");
    Serial.print(gyro_z);
    Serial.println();

#endif
}

int ACSControlTask::current2PWM(float current)
{
    float voltage;
    if (!sfr::battery::voltage_value->get_value(&voltage)) {
        voltage = 0;
    }

    float abs_current = fabs(current);
    float voltage_cof = voltage * 0.24038134 + 0.02798774;
    abs_current = abs_current / voltage_cof;

    int PWM = -4474.72 * pow(abs_current, 2) + 2099.351 * abs_current + 14.17;


    if(current<0){
        PWM = -PWM;
    }

    return std::max(-255, std::min(PWM, 255));
}

void ACSControlTask::ACSWrite(int torqorder, float current, int out1, int out2, int PWMpin)
{
    int PWM = current2PWM(current);
    int abs_PWM = abs(PWM);


    if (PWMpin == constants::acs::xPWMpin) {
        sfr::acs::pwm_x = prev_PWMx;
        prev_PWMx = PWM;
    } else if (PWMpin == constants::acs::yPWMpin) {
        sfr::acs::pwm_y = prev_PWMy;
        prev_PWMy = PWM;
    } else if (PWMpin == constants::acs::zPWMpin) {
        sfr::acs::pwm_z = prev_PWMz;
        prev_PWMz = PWM;
    }

    if (current == 0) {
        digitalWrite(out1, LOW);
        digitalWrite(out2, LOW);
    } else if (torqorder == 0) {
        if (current > 0) {
            digitalWrite(out1, HIGH);
            digitalWrite(out2, LOW);
            analogWrite(PWMpin, abs_PWM);
        }else{
            digitalWrite(out1, LOW);
            digitalWrite(out2, HIGH);
            analogWrite(PWMpin, abs_PWM);
        }
    } else if (torqorder == 1) {
        if (current > 0) {
            digitalWrite(out1, LOW);
            digitalWrite(out2, HIGH);
            analogWrite(PWMpin, abs_PWM);
        }else{
            digitalWrite(out1, HIGH);
            digitalWrite(out2, LOW);
            analogWrite(PWMpin, abs_PWM);
        }
    }

}

void ACSControlTask::ACSWrite_PWM(int torqorder, int PWM, int out1, int out2, int PWMpin)
{

    int PWM_abs = abs(PWM);

    if (PWMpin == constants::acs::xPWMpin) {
        sfr::acs::pwm_x = prev_PWMx;
        prev_PWMx = PWM;
    } else if (PWMpin == constants::acs::yPWMpin) {
        sfr::acs::pwm_y = prev_PWMy;
        prev_PWMy = PWM;
    } else if (PWMpin == constants::acs::zPWMpin) {
        sfr::acs::pwm_z = prev_PWMz;
        prev_PWMz = PWM;
    }

    if (PWM == 0) {
        digitalWrite(out1, LOW);
        digitalWrite(out2, LOW);
    } else if (torqorder == 0) {
        if (PWM > 0) {
            digitalWrite(out1, HIGH);
            digitalWrite(out2, LOW);
            analogWrite(PWMpin, PWM_abs);
        }
        if (PWM < 0) {
            digitalWrite(out1, LOW);
            digitalWrite(out2, HIGH);
            analogWrite(PWMpin, PWM_abs);
        }
    } else if (torqorder == 1) {
        if (PWM > 0) {
            digitalWrite(out1, LOW);
            digitalWrite(out2, HIGH);
            analogWrite(PWMpin, PWM_abs);
        }
        if (PWM < 0) {
            digitalWrite(out1, HIGH);
            digitalWrite(out2, LOW);
            analogWrite(PWMpin, PWM_abs);
        }
    }
}

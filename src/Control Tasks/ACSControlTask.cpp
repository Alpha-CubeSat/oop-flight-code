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

            } else if (sfr::acs::mode == (uint8_t)acs_mode_type::point) {
                sfr::acs::current_x = starshotObj.rtY.point[0];
                sfr::acs::current_y = starshotObj.rtY.point[1];

                sfr::acs::current_z = starshotObj.rtY.point[2];
            } else if (sfr::acs::mode == (uint8_t)acs_mode_type::simple) {
                sfr::acs::current_x = 0;
                sfr::acs::current_y = 0;
                sfr::acs::current_z = 0;
                if (sfr::acs::simple_mag == (uint8_t)mag_type::x) {
                    sfr::acs::current_x = sfr::acs::simple_current;
                } else if (sfr::acs::simple_mag == (uint8_t)mag_type::y) {
                    sfr::acs::current_y = sfr::acs::simple_current;
                } else if (sfr::acs::simple_mag == (uint8_t)mag_type::z) {
                    sfr::acs::current_z = sfr::acs::simple_current;
                }
            }
        }
    }

    if (sfr::acs::off || !imu_valid) {
        sfr::acs::current_x = 0;
        sfr::acs::current_y = 0;
        sfr::acs::current_z = 0;
    }

    ACSWrite(constants::acs::xtorqorder, sfr::acs::current_x, constants::acs::xout1, constants::acs::xout2, constants::acs::xPWMpin);
    ACSWrite(constants::acs::ytorqorder, sfr::acs::current_y, constants::acs::yout1, constants::acs::yout2, constants::acs::yPWMpin);
    ACSWrite(constants::acs::ztorqorder, sfr::acs::current_z, constants::acs::zout1, constants::acs::zout2, constants::acs::zPWMpin);

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
        Serial.print("POINT, ");
    } else if (sfr::acs::mode == 2) {
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
    Serial.print(sfr::acs::current_x);
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
    if (int(633.5 * pow(fabs(current), 0.6043) + 8.062) < 8.062)
        return 0;
    else if (int(633.5 * pow(fabs(current), 0.6043) + 8.062) > 255)
        return 255;
    else
        return int(633.5 * pow(fabs(current), 0.6043) + 8.062);
}

void ACSControlTask::ACSWrite(int torqorder, float current, int out1, int out2, int PWMpin)
{
    int PWM = current2PWM(current);
#ifdef VERBOSE
    Serial.print("PWM: ");
    Serial.println(PWM);
#endif
    if (PWMpin == constants::acs::xPWMpin) {
        sfr::acs::pwm_x = PWM;
    } else if (PWMpin == constants::acs::yPWMpin) {
        sfr::acs::pwm_y = PWM;
    } else if (PWMpin == constants::acs::zPWMpin) {
        sfr::acs::pwm_z = PWM;
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

#include "ACSMonitor.hpp"
ACSMonitor::ACSMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    using namespace sfr::acs;
    starshotObj.initialize(kane_damper_c, kane_Id,
                           ampfactor, max_current, csarea, num_loops, wdx, wdy, wdz);
}

float x_new = 0;
float x_old = 0;
float x_oldLP = 0;
float x_newLP = 0;

float y_new = 0;
float y_old = 0;
float y_oldLP = 0;
float y_newLP = 0;

float z_new = 0;
float z_old = 0;
float z_oldLP = 0;
float z_newLP = 0;

float wx_new = 0;
float wx_old = 0;
float wx_oldLP = 0;
float wx_newLP = 0;

float wy_new = 0;
float wy_old = 0;
float wy_oldLP = 0;
float wy_newLP = 0;

float wz_new = 0;
float wz_old = 0;
float wz_oldLP = 0;
float wz_newLP = 0;

// 6th order low pass filter, cutoff frequency of 0.1 Hz
float x_a = 0.43708596;
float x_b[] = {0.28145702, 0.28145702};

float LP_filter(float old_LPval, float new_val, float old_val, float a, float b[])
{
    float new_LPval = a * old_LPval + b[0] * new_val + b[1] * old_val;
    return new_LPval;
}

void ACSMonitor::execute()

{
    Serial.print("ACS called, current MissionMode:");
    Serial.println(sfr::mission::current_mode->get_id());
    // loading adjusted values into the ACS model
    float w_x_raw = sfr::imu::gyro_x_average->get_value() * 3.14159 / 180.0;
    float w_y_raw = sfr::imu::gyro_y_average->get_value() * 3.14159 / 180.0;
    float w_z_raw = sfr::imu::gyro_z_average->get_value() * 3.14159 / 180.0;

    float w_x = w_x_raw;
    float w_y = w_y_raw;
    float w_z = w_z_raw;

    // float mag_x_raw = sfr::imu::mag_x_average->get_value();
    // float mag_y_raw = sfr::imu::mag_y_average->get_value();
    // float mag_z_raw = sfr::imu::mag_z_average->get_value();

    float mag_x_raw = sfr::imu::mag_x;
    float mag_y_raw = sfr::imu::mag_y;
    float mag_z_raw = sfr::imu::mag_z;

    float mag_x = mag_x_raw;
    float mag_y = mag_y_raw;
    float mag_z = mag_z_raw;

    Serial.print("magXYZ w/o offset(raw):");
    Serial.print(mag_x);
    Serial.print(", ");
    Serial.print(mag_y);
    Serial.print(", ");
    Serial.println(mag_z);

    //IMUOffset(&mag_x, &mag_y, &mag_z, sfr::temperature::temp_c, sfr::battery::voltage, sfr::acs::pwmX, sfr::acs::pwmY, sfr::acs::pwmZ);

    x_new = mag_x;
    x_newLP = LP_filter(x_oldLP, x_new, x_old, x_a, x_b);

    y_new = mag_y;
    y_newLP = LP_filter(y_oldLP, y_new, y_old, x_a, x_b);

    z_new = mag_z;
    z_newLP = LP_filter(z_oldLP, z_new, z_old, x_a, x_b);


    mag_x = x_newLP;
    mag_y = y_newLP;
    mag_z = z_newLP;

    wx_new = w_x;
    wx_newLP = LP_filter(wx_oldLP, wx_new, wx_old, x_a, x_b);

    wy_new = w_y;
    wy_newLP = LP_filter(wy_oldLP, wy_new, wy_old, x_a, x_b);

    wz_new = w_z;
    wz_newLP = LP_filter(wz_oldLP, wz_new, wz_old, x_a, x_b);

    w_x = wx_newLP;
    w_y = wy_newLP;
    w_z = wz_newLP;

    Serial.print("magXYZ smooth");
    Serial.print(mag_x);
    Serial.print(", ");
    Serial.print(mag_y);
    Serial.print(", ");
    Serial.println(mag_z);

    // Convert from uT to T
    mag_x = mag_x/ 1000000.0;
    mag_y = mag_y/ 1000000.0;
    mag_z = mag_z/ 1000000.0;
    

    Serial.print("wXYZ: ");
    Serial.print(w_x);
    Serial.print(", ");
    Serial.print(w_y);
    Serial.print(", ");
    Serial.println(w_z);

    starshotObj.rtU.w[0] = w_x;
    starshotObj.rtU.w[1] = w_y;
    starshotObj.rtU.w[2] = w_z;

    starshotObj.rtU.magneticfield[0] = mag_x;
    starshotObj.rtU.magneticfield[1] = mag_y;
    starshotObj.rtU.magneticfield[2] = mag_z;

    starshotObj.step();

    float de_I_x = starshotObj.rtY.detumble[0];
    float de_I_y = starshotObj.rtY.detumble[1];
    float de_I_z = starshotObj.rtY.detumble[2];

    float pt_I_x = starshotObj.rtY.point[0];
    float pt_I_y = starshotObj.rtY.point[1];
    float pt_I_z = starshotObj.rtY.point[2];

    // testing pointing mode
    digitalWrite(constants::acs::STBXYpin,HIGH);
    digitalWrite(constants::acs::STBZpin,HIGH);
    sfr::acs::mode = acs_mode_type::point;

    //sfr::acs::mag = simple_acs_type::z;

    switch (sfr::acs::mode) {
    case acs_mode_type::detumble:
        Serial.println("In detumble mode!, X, Y, Z current: ");
        Serial.print(de_I_x * 1000);
        Serial.print(", ");
        Serial.print(de_I_y* 1000);
        Serial.print(", ");
        Serial.print(de_I_z* 1000);
        Serial.println("");

        sfr::acs::currentX = de_I_x;
        sfr::acs::currentY = de_I_y;
        sfr::acs::currentZ = de_I_z;
        break;

    case acs_mode_type::simple:
        switch (sfr::acs::mag) {
        case simple_acs_type::z:
            sfr::acs::currentX = 0;
            sfr::acs::currentY = 0;
            sfr::acs::currentZ = sfr::acs::simpleCurrent;
            break;
        case simple_acs_type::x:
            sfr::acs::currentX = sfr::acs::simpleCurrent;
            sfr::acs::currentY = 0;
            sfr::acs::currentZ = 0;
            break;
        case simple_acs_type::y:
            sfr::acs::currentX = 0;
            sfr::acs::currentY = sfr::acs::simpleCurrent;
            sfr::acs::currentZ = 0;
            break;
        }
        break;
    case acs_mode_type::point:
        Serial.println("In pointing mode!, X, Y, Z current: ");
        Serial.print(pt_I_x * 1000);
        Serial.print(" mA, ");
        Serial.print(pt_I_y * 1000);
        Serial.print(" mA, ");
        Serial.print(pt_I_z * 1000);
        Serial.println(" mA");

        sfr::acs::currentX = pt_I_x;
        sfr::acs::currentY = pt_I_y;
        sfr::acs::currentZ = pt_I_z;
        break;
    case acs_mode_type::off:

        Serial.println("PT X, Y, Z current: ");
        Serial.print(pt_I_x * 1000);
        Serial.print(" mA, ");
        Serial.print(pt_I_y * 1000);
        Serial.print(" mA, ");
        Serial.print(pt_I_z * 1000);
        Serial.println(" mA");
        sfr::acs::currentX = 0;
        sfr::acs::currentY = 0;
        sfr::acs::currentZ = 0;
        break;
    }

    x_old = x_new;
    x_oldLP = x_newLP;
    y_old = y_new;
    y_oldLP = y_newLP;
    z_old = z_new;
    z_oldLP = z_newLP;

    wx_old = wx_new;
    wx_oldLP = wx_newLP;
    wy_old = wy_new;
    wy_oldLP = wy_newLP;
    wz_old = wz_new;
    wz_oldLP = wz_newLP;

    //float ACSData[12] = {w_x, w_y, w_z, mag_x, mag_y, mag_z, de_I_x, de_I_y, de_I_z, pt_I_x, pt_I_y, pt_I_z};
    float ACSData[13] = {mag_x_raw, mag_y_raw, mag_z_raw, mag_x * 1000000.0, mag_y * 1000000.0, mag_z * 1000000.0, w_x_raw * 10000.0, w_y_raw * 10000.0, w_z_raw * 10000.0, w_x * 10000.0, w_y * 10000.0, w_z * 10000.0, pt_I_z * 1000.0};
    DataLog(ACSData, 13);
}

void ACSMonitor::IMUOffset(float *mag_x, float *mag_y, float *mag_z, float temp, float voltage, float pwmX, float pwmY, float pwmZ)
{
    using namespace constants::acs;

    /*Offset Contributions from PWM (ex: pwmX_oX is contribution of X mag to offset x)*/
    float pwmX_ox = pwmX_ox_1 * pwmX + pwmX_ox_2 * pow(pwmX, 2) + pwmX_ox_3 * pow(pwmX, 3);
    float pwmX_oy = pwmX_oy_1 * pwmX + pwmX_oy_2 * pow(pwmX, 2) + pwmX_oy_3 * pow(pwmX, 3);
    float pwmX_oz = pwmX_oz_1 * pwmX + pwmX_oz_2 * pow(pwmX, 2) + pwmX_oz_3 * pow(pwmX, 3);

    float pwmY_ox = pwmY_ox_1 * pwmY + pwmY_ox_2 * pow(pwmY, 2) + pwmY_ox_3 * pow(pwmY, 3);
    float pwmY_oy = pwmY_oy_1 * pwmY + pwmY_oy_2 * pow(pwmY, 2) + pwmY_oy_3 * pow(pwmY, 3);
    float pwmY_oz = pwmY_oz_1 * pwmY + pwmY_oz_2 * pow(pwmY, 2) + pwmY_oz_3 * pow(pwmY, 3);

    float pwmZ_ox = pwmZ_ox_1 * pwmZ + pwmZ_ox_2 * pow(pwmZ, 2) + pwmZ_ox_3 * pow(pwmZ, 3);
    float pwmZ_oy = pwmZ_oy_1 * pwmZ + pwmZ_oy_2 * pow(pwmZ, 2) + pwmZ_oy_3 * pow(pwmZ, 3);
    float pwmZ_oz = pwmZ_oz_1 * pwmZ + pwmZ_oz_2 * pow(pwmZ, 2) + pwmZ_oz_3 * pow(pwmZ, 3);
    /*******************************************/
    /*Voltage Adjustment Coefficients (ex: volX_ox = coef for pwmX_oX)*/
    float volX_ox = volX_ox_1 * voltage + volX_ox_c;
    float volX_oy = volX_oy_1 * voltage + volX_oy_c;
    float volX_oz = volX_oz_1 * voltage + volX_oz_c;

    float volY_ox = volY_ox_1 * voltage + volY_ox_c;
    float volY_oy = volY_oy_1 * voltage + volY_oy_c;
    float volY_oz = volY_oz_1 * voltage + volY_oz_c;

    float volZ_ox = volZ_ox_1 * voltage + volZ_ox_c;
    float volZ_oy = volZ_oy_1 * voltage + volZ_oy_c;
    float volZ_oz = volZ_oz_1 * voltage + volZ_oz_c;
    /*******************************************/
    /*Temperature Offset Terms*/
    float temp_x = temp_x_1 * temp + temp_x_2 * pow(temp, 2) + temp_x_3 * pow(temp, 3) + temp_x_c;
    float temp_y = temp_y_1 * temp + temp_y_2 * pow(temp, 2) + temp_y_3 * pow(temp, 3) + temp_y_c;
    float temp_z = temp_z_1 * temp + temp_z_2 * pow(temp, 2) + temp_z_3 * pow(temp, 3) + temp_z_c;
    /*******************************************/
    /*Total Offsets*/
    float xoffset = volX_ox * pwmX_ox + volY_ox * pwmY_ox + volZ_ox * pwmZ_ox + temp_x + hardiron_x;
    float yoffset = volX_oy * pwmX_oy + volY_oy * pwmY_oy + volZ_oy * pwmZ_oy + temp_y + hardiron_y;
    float zoffset = volX_oz * pwmX_oz + volY_oz * pwmY_oz + volZ_oz * pwmZ_oz + temp_z + hardiron_z;
    /*******************************************/
    /* Finally, adjust magnetometer readings*/

    *mag_x = *mag_x - xoffset;
    *mag_y = *mag_y - yoffset;
    *mag_z = *mag_z - zoffset;
}

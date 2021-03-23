#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

namespace constants
{
    namespace photoresistor
    {
        constexpr int pin = 22;
        constexpr int light_val = 0;
    }
    namespace burnwire
    {
        constexpr int first_pin = 35;
        constexpr int second_pin = 0;
        constexpr int burn_time = 500;
        constexpr int burn_wait = 1000;
        constexpr int max_attempts = 10;
    }
    namespace rockblock
    {
        constexpr int baud = 19200;
        constexpr unsigned long two_hours = 7200000;
        constexpr unsigned long ten_minutes = 600000;
        constexpr int buffer_size = 63;
        constexpr int packet_size = 70;
        constexpr int num_commas = 5;
    }
    namespace temperature
    {
        constexpr int pin = 16;
    }
    namespace current
    {
        constexpr int pin = 15;
    }
    namespace acs
    {
        constexpr int yout1 = 5;
        constexpr int yout2 = 6;
        constexpr int zout1 = 9;
        constexpr int zout2 = 10;
        constexpr int xout1 = 7;
        constexpr int xout2 = 8;
        constexpr int xPWMpin = 2;
        constexpr int yPWMpin = 3;
        constexpr int zPWMpin = 4;
        constexpr int STBYpin = 21;
        constexpr int STBZpin = 20;
        constexpr int xtorqorder = 1;
        constexpr int ytorqorder = 1;
        constexpr int ztorqorder = 1;
    }
    namespace battery
    {
        constexpr int pin = 32;
    }
    namespace fault
    {
        //fault 1
        constexpr unsigned char mag_x = 1 << 0;
        constexpr unsigned char mag_y = 1 << 1;
        constexpr unsigned char mag_z = 1 << 2;
        constexpr unsigned char gyro_x = 1 << 3;
        constexpr unsigned char gyro_y = 1 << 4;
        constexpr unsigned char gyro_z = 1 << 5;
        constexpr unsigned char acc_x = 1 << 6;
        constexpr unsigned char acc_y = 1 << 7;

        //fault 2
        constexpr unsigned char acc_z = 1 << 0;
        constexpr unsigned char temp_c = 1 << 1;
        constexpr unsigned char solar_current = 1 << 2;
        constexpr unsigned char voltage = 1 << 3;

        //fault 3
        constexpr unsigned char burn_wire = 1 << 0;
        constexpr unsigned char sd_card = 1 << 1;
        constexpr unsigned char camera_on_failed = 1 << 2;
    }
    namespace camera
    {
        //Power Control Pin
        constexpr int power_on_pin = 24;
    }
    namespace button
    {
        constexpr int button_pin = 13;
    }
    namespace imu
    {
        constexpr float gravity = 9.80665;
        constexpr float two_g = 2 * 9.80665;
        constexpr float four_g = 4 * 9.80665;
        constexpr float eight_g = 8 * 9.80665;

        constexpr float min_mag_x = 0.0;
        constexpr float max_mag_x = 0.0;
        constexpr float min_mag_y = 0.0;
        constexpr float max_mag_y = 0.0;
        constexpr float min_mag_z = 0.0;
        constexpr float max_mag_z = 0.0;

        constexpr float min_gyro_x = 0.0;
        constexpr float max_gyro_x = 0.0;
        constexpr float min_gyro_y = 0.0;
        constexpr float max_gyro_y = 0.0;
        constexpr float min_gyro_z = 0.0;
        constexpr float max_gyro_z = 0.0;

        constexpr float min_acc_x = 0.0;
        constexpr float max_acc_x = 0.0;
        constexpr float min_acc_y = 0.0;
        constexpr float max_acc_y = 0.0;
        constexpr float min_acc_z = 0.0;
        constexpr float max_acc_z = 0.0;
    }
};

#endif

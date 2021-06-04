#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

namespace constants
{
    namespace photoresistor
    {
        constexpr int pin = 17;
        constexpr int light_val = 0;
    }
    namespace burnwire
    {
        constexpr int first_pin = 37;
        constexpr int second_pin = 35;
        constexpr int burn_time = 500;
        constexpr int burn_wait = 1000;
        constexpr int max_attempts = 10;
        
        //2 days in milliseconds
        constexpr int armed_time = 172800000;
    }
    namespace rockblock
    {
        constexpr int baud = 19200;
        constexpr unsigned long two_hours = 7200000;
        constexpr unsigned long ten_minutes = 600000;
        constexpr int buffer_size = 63;
        constexpr int packet_size = 70;
        constexpr int num_commas = 5;
        constexpr int max_iter = 200;
        //0A0B0C0D0E0F01020304
        constexpr int known_commands[19][10] = {{10,11,12,13,14,15,1,2,3,4}};
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
        constexpr int xPWMpin = 2;
        constexpr int yPWMpin = 3;
        constexpr int zPWMpin = 4;

        constexpr int yout1 = 5;
        constexpr int yout2 = 6;

        constexpr int xout1 = 7;
        constexpr int xout2 = 8;

        constexpr int zout1 = 9;
        constexpr int zout2 = 10;

        constexpr int STBXYpin = 21;
        constexpr int STBZpin = 20;
    }
    namespace battery
    {
        constexpr int voltage_value_pin = 32;
        constexpr int allow_measurement_pin = 36;
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
        //low when door is opened
        constexpr int button_pin = 31;
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

        //TODO: When are these passed in to the driver?
        constexpr int SCL = 19;
        constexpr int SDA = 18;
    }
    namespace timecontrol{
        // Environment-based initializations of the control loop time.
        // control_cycle_time is the value actually used for timing. The
        // other constants are just informational.
        constexpr unsigned int control_cycle_time_ms = 100;
        constexpr unsigned int control_cycle_time_us = control_cycle_time_ms * 1000;
        constexpr unsigned int control_cycle_time = control_cycle_time_us;


        // Control cycle time offsets, in microseconds 
        static constexpr unsigned int acs_monitor_offset = 0;
        static constexpr unsigned int battery_monitor_offset = 1*30000;
        static constexpr unsigned int button_monitor_offset = 2*30000;
        static constexpr unsigned int camera_report_monitor_offset = 3*30000;
        static constexpr unsigned int command_monitor_offset = 4*30000;
        static constexpr unsigned int current_monitor_offset = 5*30000;
        static constexpr unsigned int fault_monitor_offset = 6*30000;
        static constexpr unsigned int imu_monitor_offset = 7*30000;
        static constexpr unsigned int normal_report_monitor_offset = 8*30000;
        static constexpr unsigned int photoresistor_monitor_offset = 9*30000;
        static constexpr unsigned int temperature_monitor_offset = 10*30000;
        
        static constexpr unsigned int acs_control_task_offset = 11*30000;
        static constexpr unsigned int burnwire_control_task_offset = 12*30000;
        static constexpr unsigned int camera_control_task_offset = 13*30000;
        static constexpr unsigned int rockblock_control_task_offset = 14*30000;
        static constexpr unsigned int temperature_control_task_offset = 15*30000;

        static constexpr unsigned int mission_manager_offset = 16*30000;
    }
};

#endif

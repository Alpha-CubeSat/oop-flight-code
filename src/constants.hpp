#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

namespace constants
{
    namespace sensor
    {
        constexpr int collect = 3;
    }
    namespace photoresistor
    {
        constexpr int pin = 38;
        constexpr int light_val = 150;
    }
    namespace burnwire
    {
        constexpr int first_pin = 14;
        constexpr int second_pin = 15;
        constexpr int burn_wait = 1000;
        constexpr int max_attempts = 10;
        constexpr int camera_max_attempts = 50;
        constexpr int min_burnwire_time = 0;
        constexpr int max_burnwire_time = 60000;
        constexpr int min_armed_time = 0;
        constexpr int max_armed_time = 86400000;
    }
    namespace rockblock
    {   
        constexpr unsigned long one_second = 1000;
        constexpr unsigned long one_minute = 60 * one_second;
        constexpr unsigned long one_hour = 60 * one_minute;
        constexpr unsigned long one_day = 24 * one_hour;

        constexpr unsigned long half_second = one_second / 2;
        constexpr unsigned long ten_minutes = 10 * one_minute;
        constexpr unsigned long two_hours = 2 * one_hour;
        constexpr unsigned long two_days = 2 * one_day;

        constexpr int sleep_pin = 19;

        constexpr int min_sleep_period = 2 * one_minute;

        constexpr int min_downlink_period = one_second;
        constexpr int max_downlink_period = two_days;

        constexpr int baud = 19200;
        constexpr size_t buffer_size = 63;
        constexpr size_t packet_size = 70;
        constexpr size_t num_commas = 5;
        constexpr size_t max_iter = 200;
        constexpr size_t num_commands = 41;
        constexpr size_t opcode_len = 2;
        constexpr size_t arg1_len = 4;
        constexpr size_t arg2_len = 4;
        constexpr size_t command_len = opcode_len + arg1_len + arg2_len;
        constexpr size_t max_conseq_read = 3;
        
        constexpr int num_initial_downlinks = 2;

        constexpr uint8_t mission_mode[opcode_len] =               {0x00,0x00};
        constexpr uint8_t burnwire_arm[opcode_len] =               {0x01,0x00};
        constexpr uint8_t burnwire_fire[opcode_len] =              {0x02,0x00};
        constexpr uint8_t burnwire_time[opcode_len] =              {0x03,0x00};
        constexpr uint8_t burnwire_timeout[opcode_len] =           {0x04,0x00};
        constexpr uint8_t rockblock_downlink_period[opcode_len] =  {0x05,0x00};
        constexpr uint8_t request_image_fragment[opcode_len] =     {0x06,0x00};
        constexpr uint8_t camera_take_photo[opcode_len] =          {0x07,0x00};
        constexpr uint8_t temperature_mode[opcode_len] =           {0x08,0x00};
        constexpr uint8_t acs_mode[opcode_len] =                   {0x09,0x00};
        constexpr uint8_t change_simplified_acs[opcode_len] =      {0x0A,0x00};
        constexpr uint8_t camera_turn_on[opcode_len] =             {0x0B,0x00};
        constexpr uint8_t camera_turn_off[opcode_len] =            {0x0C,0x00};
        constexpr uint8_t fault_mode[opcode_len] =                 {0xF1,0xFF};
        constexpr uint8_t fault_check_mag_x[opcode_len] =          {0xF2,0xFF};
        constexpr uint8_t fault_check_mag_y[opcode_len] =          {0xF3,0xFF};
        constexpr uint8_t fault_check_mag_z[opcode_len] =          {0xF4,0xFF};
        constexpr uint8_t fault_check_gyro_x[opcode_len] =         {0xF5,0xFF};
        constexpr uint8_t fault_check_gyro_y[opcode_len] =         {0xF6,0xFF};
        constexpr uint8_t fault_check_gyro_z[opcode_len] =         {0xF7,0xFF};
        constexpr uint8_t fault_check_temp_c[opcode_len] =         {0xF8,0xFF};
        constexpr uint8_t fault_check_solar_current[opcode_len] =  {0xF9,0xFF};
        constexpr uint8_t fault_check_voltage[opcode_len] =        {0xFA,0xFF};

        constexpr uint8_t initialization[arg1_len] ={0x00,0x00,0x00,0x00};
        constexpr uint8_t low_power[arg1_len] =     {0x01,0x00,0x00,0x00};
        constexpr uint8_t deployment[arg1_len]=     {0x02,0x00,0x00,0x00};
        constexpr uint8_t standby[arg1_len] =       {0x03,0x00,0x00,0x00};
        constexpr uint8_t safe[arg1_len] =          {0x03,0x00,0x00,0x00};
        constexpr uint8_t true_arg[arg1_len] =      {0x01,0x00,0x00,0x00};
        constexpr uint8_t false_arg[arg1_len] =     {0x00,0x00,0x00,0x00};
        constexpr uint8_t active[arg1_len] =        {0x01,0x00,0x00,0x00};
        constexpr uint8_t inactive[arg1_len] =      {0x00,0x00,0x00,0x00};
        constexpr uint8_t full[arg1_len] =          {0x02,0x00,0x00,0x00};
        constexpr uint8_t simple[arg1_len] =        {0x01,0x00,0x00,0x00};
        constexpr uint8_t off[arg1_len] =           {0x00,0x00,0x00,0x00};
        constexpr uint8_t x[arg1_len] =             {0x00,0x00,0x00,0x00};
        constexpr uint8_t y[arg1_len] =             {0x01,0x00,0x00,0x00};
        constexpr uint8_t z[arg1_len] =             {0x02,0x00,0x00,0x00};

        constexpr uint8_t no_arg_2[arg2_len] =      {0x00,0x00,0x00,0x00};

        constexpr uint8_t mission_mode_low_power[command_len] = {
            mission_mode[0], mission_mode[1], 
            low_power[0], low_power[1], low_power[2], low_power[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]
        };

        constexpr uint8_t mission_mode_deployment[command_len] = {
            mission_mode[0], mission_mode[1], 
            deployment[0], deployment[1], deployment[2], deployment[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]
        };

        constexpr uint8_t mission_mode_standby[command_len] = {
            mission_mode[0], mission_mode[1], 
            standby[0], standby[1], standby[2], standby[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]
        };
        
        constexpr uint8_t mission_mode_safe[command_len] = {
            mission_mode[0], mission_mode[1], 
            safe[0], safe[1], safe[2], safe[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]
        };

        constexpr uint8_t mission_mode_initialization[command_len] = {
            mission_mode[0], mission_mode[1], 
            initialization[0], initialization[1], initialization[2], initialization[3],
            no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]
        };

        constexpr uint8_t burnwire_arm_true[command_len] = {
            burnwire_arm[0], burnwire_arm[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };   

        constexpr uint8_t burnwire_arm_false[command_len] = {
            burnwire_arm[0], burnwire_arm[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };    

        constexpr uint8_t burnwire_fire_true[command_len] = {
            burnwire_fire[0], burnwire_fire[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };  

        constexpr uint8_t burnwire_fire_false[command_len] = {
            burnwire_fire[0], burnwire_fire[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };    
 
        constexpr uint8_t camera_take_photo_true[command_len] = {
            camera_take_photo[0], camera_take_photo[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };    

        constexpr uint8_t camera_take_photo_false[command_len] = {
            camera_take_photo[0], camera_take_photo[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };  

        constexpr uint8_t temperature_mode_active[command_len] = {
            temperature_mode[0], temperature_mode[1], 
            active[0], active[1], active[2], active[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t temperature_mode_inactive[command_len] = {
            temperature_mode[0], temperature_mode[1], 
            inactive[0], inactive[1], inactive[2], inactive[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };   

        constexpr uint8_t acs_mode_full[command_len] = {
            acs_mode[0], acs_mode[1], 
            full[0], full[1], full[2], full[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };  

        constexpr uint8_t acs_mode_simple[command_len] = {
            acs_mode[0], acs_mode[1], 
            simple[0], simple[1], simple[2], simple[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };  

        constexpr uint8_t acs_mode_off[command_len] = {
            acs_mode[0], acs_mode[1], 
            off[0], off[1], off[2], off[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };   

        constexpr uint8_t fault_mode_active[command_len] = {
            fault_mode[0], fault_mode[1], 
            active[0], active[1], active[2], active[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_mode_inactive[command_len] = {
            fault_mode[0], fault_mode[1], 
            inactive[0], inactive[1], inactive[2], inactive[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };                    

        constexpr uint8_t fault_check_mag_x_true[command_len] = {
            fault_check_mag_x[0], fault_check_mag_x[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_mag_x_false[command_len] = {
            fault_check_mag_x[0], fault_check_mag_x[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_mag_y_true[command_len] = {
            fault_check_mag_y[0], fault_check_mag_y[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_mag_y_false[command_len] = {
            fault_check_mag_y[0], fault_check_mag_y[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 
        
        constexpr uint8_t fault_check_mag_z_true[command_len] = {
            fault_check_mag_z[0], fault_check_mag_z[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_mag_z_false[command_len] = {
            fault_check_mag_z[0], fault_check_mag_z[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };

        constexpr uint8_t fault_check_gyro_x_true[command_len] = {
            fault_check_gyro_x[0], fault_check_gyro_x[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_gyro_x_false[command_len] = {
            fault_check_gyro_x[0], fault_check_gyro_x[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_gyro_y_true[command_len] = {
            fault_check_gyro_y[0], fault_check_gyro_y[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_gyro_y_false[command_len] = {
            fault_check_gyro_y[0], fault_check_gyro_y[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 
        
        constexpr uint8_t fault_check_gyro_z_true[command_len] = {
            fault_check_gyro_z[0], fault_check_gyro_z[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_gyro_z_false[command_len] = {
            fault_check_gyro_z[0], fault_check_gyro_z[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_temp_c_true[command_len] = {
            fault_check_temp_c[0], fault_check_temp_c[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_temp_c_false[command_len] = {
            fault_check_temp_c[0], fault_check_temp_c[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_solar_current_true[command_len] = {
            fault_check_solar_current[0], fault_check_solar_current[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_solar_current_false[command_len] = {
            fault_check_solar_current[0], fault_check_solar_current[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_voltage_true[command_len] = {
            fault_check_voltage[0], fault_check_voltage[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        }; 

        constexpr uint8_t fault_check_voltage_false[command_len] = {
            fault_check_voltage[0], fault_check_voltage[1], 
            false_arg[0], false_arg[1], false_arg[2], false_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };

        constexpr uint8_t change_simplified_acs_x[command_len] = {
            change_simplified_acs[0], change_simplified_acs[1], 
            x[0], x[1], x[2], x[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };

        constexpr uint8_t change_simplified_acs_y[command_len] = {
            change_simplified_acs[0], change_simplified_acs[1], 
            y[0], y[1], y[2], y[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };

        constexpr uint8_t change_simplified_acs_z[command_len] = {
            change_simplified_acs[0], change_simplified_acs[1], 
            z[0], z[1], z[2], z[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };

        constexpr uint8_t camera_turn_on_true[command_len] = {
            camera_turn_on[0], camera_turn_on[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };

        constexpr uint8_t camera_turn_off_true[command_len] = {
            camera_turn_off[0], camera_turn_off[1], 
            true_arg[0], true_arg[1], true_arg[2], true_arg[3],
            no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]
        };

        constexpr int known_commands[num_commands][command_len] = {
            {mission_mode_low_power[0], mission_mode_low_power[1], mission_mode_low_power[2], mission_mode_low_power[3], mission_mode_low_power[4], mission_mode_low_power[5], mission_mode_low_power[6], mission_mode_low_power[7], mission_mode_low_power[8], mission_mode_low_power[9]},
            {mission_mode_deployment[0], mission_mode_deployment[1], mission_mode_deployment[2], mission_mode_deployment[3], mission_mode_deployment[4], mission_mode_deployment[5],mission_mode_deployment[6], mission_mode_deployment[7], mission_mode_deployment[8], mission_mode_deployment[9]},
            {mission_mode_standby[0], mission_mode_standby[1], mission_mode_standby[2], mission_mode_standby[3], mission_mode_standby[4], mission_mode_standby[5], mission_mode_standby[6], mission_mode_standby[7], mission_mode_standby[8], mission_mode_standby[9]},
            {mission_mode_safe[0], mission_mode_safe[1], mission_mode_safe[2], mission_mode_safe[3], mission_mode_safe[4], mission_mode_safe[5], mission_mode_safe[6], mission_mode_safe[7], mission_mode_safe[8], mission_mode_safe[9]},          
            {mission_mode_initialization[0], mission_mode_initialization[1], mission_mode_initialization[2], mission_mode_initialization[3], mission_mode_initialization[4], mission_mode_initialization[5], mission_mode_initialization[6], mission_mode_initialization[7], mission_mode_initialization[8], mission_mode_initialization[9]},
            {burnwire_arm_true[0], burnwire_arm_true[1], burnwire_arm_true[2], burnwire_arm_true[3], burnwire_arm_true[4], burnwire_arm_true[5], burnwire_arm_true[6], burnwire_arm_true[7], burnwire_arm_true[8], burnwire_arm_true[9]},
            {burnwire_arm_false[0], burnwire_arm_false[1], burnwire_arm_false[2], burnwire_arm_false[3], burnwire_arm_false[4], burnwire_arm_false[5], burnwire_arm_false[6], burnwire_arm_false[7], burnwire_arm_false[8], burnwire_arm_false[9]},
            {burnwire_fire_true[0], burnwire_fire_true[1], burnwire_fire_true[2], burnwire_fire_true[3], burnwire_fire_true[4], burnwire_fire_true[5], burnwire_fire_true[6], burnwire_fire_true[7], burnwire_fire_true[8], burnwire_fire_true[9]},
            {burnwire_fire_false[0], burnwire_fire_false[1], burnwire_fire_false[2], burnwire_fire_false[3], burnwire_fire_false[4], burnwire_fire_false[5], burnwire_fire_false[6], burnwire_fire_false[7], burnwire_fire_false[8], burnwire_fire_false[9]},
            {camera_take_photo_true[0], camera_take_photo_true[1], camera_take_photo_true[2], camera_take_photo_true[3], camera_take_photo_true[4], camera_take_photo_true[5], camera_take_photo_true[6], camera_take_photo_true[7], camera_take_photo_true[8], camera_take_photo_true[9]},
            {camera_take_photo_false[0], camera_take_photo_false[1], camera_take_photo_false[2], camera_take_photo_false[3], camera_take_photo_false[4], camera_take_photo_false[5], camera_take_photo_false[6], camera_take_photo_false[7], camera_take_photo_false[8], camera_take_photo_false[9]},
            {temperature_mode_active[0], temperature_mode_active[1], temperature_mode_active[2], temperature_mode_active[3], temperature_mode_active[4], temperature_mode_active[5], temperature_mode_active[6], temperature_mode_active[7], temperature_mode_active[8], temperature_mode_active[9]},
            {temperature_mode_inactive[0], temperature_mode_inactive[1], temperature_mode_inactive[2], temperature_mode_inactive[3], temperature_mode_inactive[4], temperature_mode_inactive[5], temperature_mode_inactive[6], temperature_mode_inactive[7], temperature_mode_inactive[8], temperature_mode_inactive[9]},
            {acs_mode_full[0], acs_mode_full[1], acs_mode_full[2], acs_mode_full[3], acs_mode_full[4], acs_mode_full[5], acs_mode_full[6], acs_mode_full[7], acs_mode_full[8], acs_mode_full[9]},
            {acs_mode_simple[0], acs_mode_simple[1], acs_mode_simple[2], acs_mode_simple[3], acs_mode_simple[4], acs_mode_simple[5], acs_mode_simple[6], acs_mode_simple[7], acs_mode_simple[8], acs_mode_simple[9]},
            {acs_mode_off[0], acs_mode_off[1], acs_mode_off[2], acs_mode_off[3], acs_mode_off[4], acs_mode_off[5], acs_mode_off[6], acs_mode_off[7], acs_mode_off[8], acs_mode_off[9]},
            {fault_mode_active[0], fault_mode_active[1], fault_mode_active[2], fault_mode_active[3], fault_mode_active[4], fault_mode_active[5], fault_mode_active[6], fault_mode_active[7], fault_mode_active[8], fault_mode_active[9]},
            {fault_mode_inactive[0], fault_mode_inactive[1], fault_mode_inactive[2], fault_mode_inactive[3], fault_mode_inactive[4], fault_mode_inactive[5], fault_mode_inactive[6], fault_mode_inactive[7], fault_mode_inactive[8], fault_mode_inactive[9]},
            {fault_check_mag_x_true[0], fault_check_mag_x_true[1], fault_check_mag_x_true[2], fault_check_mag_x_true[3], fault_check_mag_x_true[4], fault_check_mag_x_true[5], fault_check_mag_x_true[6], fault_check_mag_x_true[7], fault_check_mag_x_true[8], fault_check_mag_x_true[9]},
            {fault_check_mag_x_false[0], fault_check_mag_x_false[1], fault_check_mag_x_false[2], fault_check_mag_x_false[3], fault_check_mag_x_false[4], fault_check_mag_x_false[5], fault_check_mag_x_false[6], fault_check_mag_x_false[7], fault_check_mag_x_false[8], fault_check_mag_x_false[9]},
            {fault_check_mag_y_true[0], fault_check_mag_y_true[1], fault_check_mag_y_true[2], fault_check_mag_y_true[3], fault_check_mag_y_true[4], fault_check_mag_y_true[5], fault_check_mag_y_true[6], fault_check_mag_y_true[7], fault_check_mag_y_true[8], fault_check_mag_y_true[9]},
            {fault_check_mag_y_false[0], fault_check_mag_y_false[1], fault_check_mag_y_false[2], fault_check_mag_y_false[3], fault_check_mag_y_false[4], fault_check_mag_y_false[5], fault_check_mag_y_false[6], fault_check_mag_y_false[7], fault_check_mag_y_false[8], fault_check_mag_y_false[9]},
            {fault_check_mag_z_true[0], fault_check_mag_z_true[1], fault_check_mag_z_true[2], fault_check_mag_z_true[3], fault_check_mag_z_true[4], fault_check_mag_z_true[5], fault_check_mag_z_true[6], fault_check_mag_z_true[7], fault_check_mag_z_true[8], fault_check_mag_z_true[9]},
            {fault_check_mag_z_false[0], fault_check_mag_z_false[1], fault_check_mag_z_false[2], fault_check_mag_z_false[3], fault_check_mag_z_false[4], fault_check_mag_z_false[5], fault_check_mag_z_false[6], fault_check_mag_z_false[7], fault_check_mag_z_false[8], fault_check_mag_z_false[9]},
            {fault_check_gyro_x_true[0], fault_check_gyro_x_true[1], fault_check_gyro_x_true[2], fault_check_gyro_x_true[3], fault_check_gyro_x_true[4], fault_check_gyro_x_true[5], fault_check_gyro_x_true[6], fault_check_gyro_x_true[7], fault_check_gyro_x_true[8], fault_check_gyro_x_true[9]},
            {fault_check_gyro_x_false[0], fault_check_gyro_x_false[1], fault_check_gyro_x_false[2], fault_check_gyro_x_false[3], fault_check_gyro_x_false[4], fault_check_gyro_x_false[5], fault_check_gyro_x_false[6], fault_check_gyro_x_false[7], fault_check_gyro_x_false[8], fault_check_gyro_x_false[9]},
            {fault_check_gyro_y_true[0], fault_check_gyro_y_true[1], fault_check_gyro_y_true[2], fault_check_gyro_y_true[3], fault_check_gyro_y_true[4], fault_check_gyro_y_true[5], fault_check_gyro_y_true[6], fault_check_gyro_y_true[7], fault_check_gyro_y_true[8], fault_check_gyro_y_true[9]},
            {fault_check_gyro_y_false[0], fault_check_gyro_y_false[1], fault_check_gyro_y_false[2], fault_check_gyro_y_false[3], fault_check_gyro_y_false[4], fault_check_gyro_y_false[5], fault_check_gyro_y_false[6], fault_check_gyro_y_false[7], fault_check_gyro_y_false[8], fault_check_gyro_y_false[9]},
            {fault_check_gyro_z_true[0], fault_check_gyro_z_true[1], fault_check_gyro_z_true[2], fault_check_gyro_z_true[3], fault_check_gyro_z_true[4], fault_check_gyro_z_true[5], fault_check_gyro_z_true[6], fault_check_gyro_z_true[7], fault_check_gyro_z_true[8], fault_check_gyro_z_true[9]},
            {fault_check_gyro_z_false[0], fault_check_gyro_z_false[1], fault_check_gyro_z_false[2], fault_check_gyro_z_false[3], fault_check_gyro_z_false[4], fault_check_gyro_z_false[5], fault_check_gyro_z_false[6], fault_check_gyro_z_false[7], fault_check_gyro_z_false[8], fault_check_gyro_z_false[9]},
            {fault_check_temp_c_true[0], fault_check_temp_c_true[1], fault_check_temp_c_true[2], fault_check_temp_c_true[3], fault_check_temp_c_true[4], fault_check_temp_c_true[5], fault_check_temp_c_true[6], fault_check_temp_c_true[7], fault_check_temp_c_true[8], fault_check_temp_c_true[9]},
            {fault_check_temp_c_false[0], fault_check_temp_c_false[1], fault_check_temp_c_false[2], fault_check_temp_c_false[3], fault_check_temp_c_false[4], fault_check_temp_c_false[5], fault_check_temp_c_false[6], fault_check_temp_c_false[7], fault_check_temp_c_false[8], fault_check_temp_c_false[9]},
            {fault_check_solar_current_true[0], fault_check_solar_current_true[1], fault_check_solar_current_true[2], fault_check_solar_current_true[3], fault_check_solar_current_true[4], fault_check_solar_current_true[5], fault_check_solar_current_true[6], fault_check_solar_current_true[7], fault_check_solar_current_true[8], fault_check_solar_current_true[9]},
            {fault_check_solar_current_false[0], fault_check_solar_current_false[1], fault_check_solar_current_false[2], fault_check_solar_current_false[3], fault_check_solar_current_false[4], fault_check_solar_current_false[5], fault_check_solar_current_false[6], fault_check_solar_current_false[7], fault_check_solar_current_false[8], fault_check_solar_current_false[9]},
            {fault_check_voltage_true[0], fault_check_voltage_true[1], fault_check_voltage_true[2], fault_check_voltage_true[3], fault_check_voltage_true[4], fault_check_voltage_true[5], fault_check_voltage_true[6], fault_check_voltage_true[7], fault_check_voltage_true[8], fault_check_voltage_true[9]},
            {fault_check_voltage_false[0], fault_check_voltage_false[1], fault_check_voltage_false[2], fault_check_voltage_false[3], fault_check_voltage_false[4], fault_check_voltage_false[5], fault_check_voltage_false[6], fault_check_voltage_false[7], fault_check_voltage_false[8], fault_check_voltage_false[9]},
            {change_simplified_acs_x[0], change_simplified_acs_x[1], change_simplified_acs_x[2], change_simplified_acs_x[3], change_simplified_acs_x[4], change_simplified_acs_x[5], change_simplified_acs_x[6], change_simplified_acs_x[7], change_simplified_acs_x[8], change_simplified_acs_x[9]},
            {change_simplified_acs_y[0], change_simplified_acs_y[1], change_simplified_acs_y[2], change_simplified_acs_y[3], change_simplified_acs_y[4], change_simplified_acs_y[5], change_simplified_acs_y[6], change_simplified_acs_y[7], change_simplified_acs_y[8], change_simplified_acs_y[9]},
            {change_simplified_acs_z[0], change_simplified_acs_z[1], change_simplified_acs_z[2], change_simplified_acs_z[3], change_simplified_acs_z[4], change_simplified_acs_z[5], change_simplified_acs_z[6], change_simplified_acs_z[7], change_simplified_acs_z[8], change_simplified_acs_z[9]},
            {camera_turn_on_true[0], camera_turn_on_true[1], camera_turn_on_true[2], camera_turn_on_true[3], camera_turn_on_true[4], camera_turn_on_true[5], camera_turn_on_true[6], camera_turn_on_true[7], camera_turn_on_true[8], camera_turn_on_true[9]},
            {camera_turn_off_true[0], camera_turn_off_true[1], camera_turn_off_true[2], camera_turn_off_true[3], camera_turn_off_true[4], camera_turn_off_true[5], camera_turn_off_true[6], camera_turn_off_true[7], camera_turn_off_true[8], camera_turn_off_true[9]}
        };
    }
    namespace temperature
    {
        constexpr int pin = 39;
        constexpr float min_temp_c;
        constexpr float max_temp_c;
        constexpr float in_sun_val;
    }
    namespace current
    {
        constexpr int pin = 22;
        // TODO: finalize min/max
        constexpr float min_solar_current = 0;
        constexpr float max_solar_current = 300;
        // TODO: finalize in_sun_val
        constexpr int in_sun_val = 70;
        constexpr float voltage_ref = 3.3;
        constexpr int resolution = 1024;
        constexpr float load = 30; // load resister value (kOhm)
        constexpr float shunt = 0.1; // shunt resistor value (Ohm)
    }
    namespace acs
    {
        constexpr int xPWMpin = 10;
        constexpr int yPWMpin = 6;
        constexpr int zPWMpin = 30;

        constexpr int yout1 = 8;
        constexpr int yout2 = 7;

        constexpr int xout1 = 25;
        constexpr int xout2 = 24;

        constexpr int zout1 = 28;
        constexpr int zout2 = 29;

        constexpr int STBXYpin = 9;
        constexpr int STBZpin = 27;

        constexpr int xtorqorder = 0;
        constexpr int ytorqorder = 0;
        constexpr int ztorqorder = 0;
    }
    namespace battery
    {
        constexpr int voltage_value_pin = 32;
        constexpr int allow_measurement_pin = 36;
        constexpr int max_voltage = 5;
        constexpr int min_voltage = 3;
        constexpr float voltage_ref = 3.3;
        constexpr int resolution = 1023;
        constexpr int r1 = 4700;
        constexpr int r2 = 10000;


    }
    namespace fault
    {
        //fault 1
        constexpr uint8_t mag_x = 1 << 0;
        constexpr uint8_t mag_y = 1 << 1;
        constexpr uint8_t mag_z = 1 << 2;
        constexpr uint8_t gyro_x = 1 << 3;
        constexpr uint8_t gyro_y = 1 << 4;
        constexpr uint8_t gyro_z = 1 << 5;
        constexpr uint8_t init = 1 << 6;

        //fault 2
        constexpr uint8_t temp_c = 1 << 1;
        constexpr uint8_t solar_current = 1 << 2;
        constexpr uint8_t voltage = 1 << 3;

        //fault 3
        constexpr uint8_t burn_wire = 1 << 0;
        constexpr uint8_t sd_card = 1 << 1;
        constexpr uint8_t camera_on_failed = 1 << 2;
    }
    namespace camera
    {
        constexpr int power_on_pin = 31;
        constexpr int content_length = 64;
        constexpr int bytes_allocated_serial_opcode = 2;
        constexpr int bytes_allocated_fragment = 4;
        constexpr int tx = 35;
        constexpr int rx = 34;
    }
    namespace button
    {
        //low when door is opened
        constexpr int button_pin = 37;
    }
    namespace imu
    {
        constexpr float gravity = 9.80665;
        constexpr float two_g = 2 * gravity;
        constexpr float four_g = 4 * gravity;
        constexpr float eight_g = 8 * gravity;

        constexpr float min_mag_x = 0.0;
        constexpr float max_mag_x = 100;
        constexpr float min_mag_y = 0.0;
        constexpr float max_mag_y = 100;
        constexpr float min_mag_z = -100;
        constexpr float max_mag_z = 100;

        constexpr float min_gyro_x = 0.0;
        constexpr float max_gyro_x = 10;
        constexpr float min_gyro_y = 0.0;
        constexpr float max_gyro_y = 10;
        constexpr float min_gyro_z = 0.0;
        constexpr float max_gyro_z = 10;

        constexpr int CSAG = 21;
        constexpr int CSM = 20;
    }
    namespace timecontrol{
        // Environment-based initializations of the control loop time.
        // control_cycle_time is the value actually used for timing. The
        // other constants are just informational.
        constexpr unsigned int control_cycle_time_ms = 250;
        constexpr unsigned int control_cycle_time_us = control_cycle_time_ms * 1000;
        constexpr unsigned int control_cycle_time = control_cycle_time_us;

        // number being added is the time length of the previous function
        // battery monitor takes max 60 us, so button monitor will start 60us after
        static constexpr unsigned int acs_monitor_offset = 0; // time starts at 0
        static constexpr unsigned int battery_monitor_offset = acs_monitor_offset + 0; // to be determined
        static constexpr unsigned int button_monitor_offset = battery_monitor_offset + 60;
        static constexpr unsigned int camera_report_monitor_offset = button_monitor_offset + 10;
        static constexpr unsigned int command_monitor_offset = camera_report_monitor_offset + 20;
        static constexpr unsigned int current_monitor_offset = command_monitor_offset + 20;
        static constexpr unsigned int fault_monitor_offset = current_monitor_offset + 50;
        static constexpr unsigned int imu_monitor_offset = fault_monitor_offset + 30;
        static constexpr unsigned int normal_report_monitor_offset = imu_monitor_offset + 9000;
        static constexpr unsigned int photoresistor_monitor_offset = normal_report_monitor_offset + 30;
        static constexpr unsigned int temperature_monitor_offset = photoresistor_monitor_offset + 30;
        
        static constexpr unsigned int acs_control_task_offset = temperature_monitor_offset + 40;
        static constexpr unsigned int burnwire_control_task_offset = acs_control_task_offset + 0; // to be determined
        static constexpr unsigned int camera_control_task_offset = burnwire_control_task_offset + 20;
        static constexpr unsigned int rockblock_control_task_offset = camera_control_task_offset + 230000;
        static constexpr unsigned int temperature_control_task_offset = rockblock_control_task_offset + 1000;
        
        static constexpr unsigned int mission_manager_offset = temperature_control_task_offset + 20;
    }
};

#endif
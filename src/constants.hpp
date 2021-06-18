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
        constexpr int camera_max_attempts = 50;
        
        //2 days in milliseconds
        constexpr int armed_time = 172800000;
    }
    namespace rockblock
    {
        //TODO
        constexpr int max_downlink_period = 0;
        constexpr int min_downlink_period = 0;

        constexpr int baud = 19200;
        constexpr unsigned long two_hours = 7200000;
        constexpr unsigned long ten_minutes = 600000;
        constexpr int buffer_size = 63;
        constexpr int packet_size = 70;
        constexpr int num_commas = 5;
        constexpr int max_iter = 200;
        constexpr int num_commands = 35;
        constexpr int opcode_len = 2;
        constexpr int arg1_len = 4;
        constexpr int arg2_len = 4;
        constexpr int command_len = opcode_len + arg1_len + arg2_len;
        constexpr int max_queue = 10;

        constexpr int mission_mode[opcode_len] =               {0x00,0x00};
        constexpr int burnwire_arm[opcode_len] =               {0x01,0x00};
        constexpr int burnwire_fire[opcode_len] =              {0x02,0x00};
        constexpr int rockblock_downlink_period[opcode_len] =  {0x03,0x00};
        constexpr int request_image_fragment[opcode_len] =     {0x04,0x00};
        constexpr int camera_take_photo[opcode_len] =          {0x05,0x00};
        constexpr int temperature_mode[opcode_len] =           {0x06,0x00};
        constexpr int acs_mode[opcode_len] =                   {0x07,0x00};
        constexpr int fault_mode[opcode_len] =                 {0xF1,0xFF};
        constexpr int fault_check_mag_x[opcode_len] =          {0xF2,0xFF};
        constexpr int fault_check_mag_y[opcode_len] =          {0xF3,0xFF};
        constexpr int fault_check_mag_z[opcode_len] =          {0xF4,0xFF};
        constexpr int fault_check_gyro_x[opcode_len] =         {0xF5,0xFF};
        constexpr int fault_check_gyro_y[opcode_len] =         {0xF6,0xFF};
        constexpr int fault_check_gyro_z[opcode_len] =         {0xF7,0xFF};
        constexpr int fault_check_temp_c[opcode_len] =         {0xF8,0xFF};
        constexpr int fault_check_solar_current[opcode_len] =  {0xF9,0xFF};
        constexpr int fault_check_voltage[opcode_len] =        {0xFA,0xFF};

        constexpr int low_power[arg1_len] =  {0x01,0x00,0x00,0x00};
        constexpr int deployment[arg1_len]=  {0x00,0x00,0x00,0x00};
        constexpr int standby[arg1_len] =    {0x02,0x00,0x00,0x00};
        constexpr int safe[arg1_len] =       {0x03,0x00,0x00,0x00};
        constexpr int true_arg[arg1_len] =   {0x01,0x00,0x00,0x00};
        constexpr int false_arg[arg1_len] =  {0x00,0x00,0x00,0x00};
        constexpr int active[arg1_len] =     {0x01,0x00,0x00,0x00};
        constexpr int inactive[arg1_len] =   {0x00,0x00,0x00,0x00};
        constexpr int detumbling[arg1_len] = {0x02,0x00,0x00,0x00};
        constexpr int pointing[arg1_len] =   {0x01,0x00,0x00,0x00};
        constexpr int off[arg1_len] =        {0x00,0x00,0x00,0x00};

        constexpr int no_arg_2[arg2_len] = {0x00,0x00,0x00,0x00};

        constexpr int mission_mode_low_power[command_len] = {mission_mode[0], mission_mode[1], 
                                                             low_power[0], low_power[1], low_power[2], low_power[3],
                                                             no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr int mission_mode_deployment[command_len] = {mission_mode[0], mission_mode[1], 
                                                              deployment[0], deployment[1], deployment[2], deployment[3],
                                                              no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr int mission_mode_standby[command_len] = {mission_mode[0], mission_mode[1], 
                                                           standby[0], standby[1], standby[2], standby[3],
                                                           no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};
        
        constexpr int mission_mode_safe[command_len] = {mission_mode[0], mission_mode[1], 
                                                        safe[0], safe[1], safe[2], safe[3],
                                                        no_arg_2[0], no_arg_2[1], no_arg_2[2], no_arg_2[3]};

        constexpr int burnwire_arm_true[command_len] = {burnwire_arm[0], burnwire_arm[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};   

        constexpr int burnwire_arm_false[command_len] = {burnwire_arm[0], burnwire_arm[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};    

        constexpr int burnwire_fire_true[command_len] = {burnwire_fire[0], burnwire_fire[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};  

        constexpr int burnwire_fire_false[command_len] = {burnwire_fire[0], burnwire_fire[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};    
 

        constexpr int camera_take_photo_true[command_len] = {camera_take_photo[0], camera_take_photo[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};    

        constexpr int camera_take_photo_false[command_len] = {camera_take_photo[0], camera_take_photo[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};  

        constexpr int temperature_mode_active[command_len] = {temperature_mode[0], temperature_mode[1], 
                                                        active[0], active[1], active[2], active[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int temperature_mode_inactive[command_len] = {temperature_mode[0], temperature_mode[1], 
                                                        inactive[0], inactive[1], inactive[2], inactive[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};   

        constexpr int acs_mode_detumbling[command_len] = {acs_mode[0], acs_mode[1], 
                                                        detumbling[0], detumbling[1], detumbling[2], detumbling[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};  

        constexpr int acs_mode_pointing[command_len] = {acs_mode[0], acs_mode[1], 
                                                        pointing[0], pointing[1], pointing[2], pointing[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};  

        constexpr int acs_mode_off[command_len] = {acs_mode[0], acs_mode[1], 
                                                        off[0], off[1], off[2], off[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};   

        constexpr int fault_mode_active[command_len] = {fault_mode[0], fault_mode[1], 
                                                        active[0], active[1], active[2], active[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_mode_inactive[command_len] = {fault_mode[0], fault_mode[1], 
                                                        inactive[0], inactive[1], inactive[2], inactive[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};                    

        constexpr int fault_check_mag_x_true[command_len] = {fault_check_mag_x[0], fault_check_mag_x[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_mag_x_false[command_len] = {fault_check_mag_x[0], fault_check_mag_x[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_mag_y_true[command_len] = {fault_check_mag_y[0], fault_check_mag_y[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_mag_y_false[command_len] = {fault_check_mag_y[0], fault_check_mag_y[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 
        
        constexpr int fault_check_mag_z_true[command_len] = {fault_check_mag_z[0], fault_check_mag_z[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_mag_z_false[command_len] = {fault_check_mag_z[0], fault_check_mag_z[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};

        constexpr int fault_check_gyro_x_true[command_len] = {fault_check_gyro_x[0], fault_check_gyro_x[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_gyro_x_false[command_len] = {fault_check_gyro_x[0], fault_check_gyro_x[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_gyro_y_true[command_len] = {fault_check_gyro_y[0], fault_check_gyro_y[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_gyro_y_false[command_len] = {fault_check_gyro_y[0], fault_check_gyro_y[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 
        
        constexpr int fault_check_gyro_z_true[command_len] = {fault_check_gyro_z[0], fault_check_gyro_z[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_gyro_z_false[command_len] = {fault_check_gyro_z[0], fault_check_gyro_z[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_temp_c_true[command_len] = {fault_check_temp_c[0], fault_check_temp_c[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_temp_c_false[command_len] = {fault_check_temp_c[0], fault_check_temp_c[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_solar_current_true[command_len] = {fault_check_solar_current[0], fault_check_solar_current[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_solar_current_false[command_len] = {fault_check_solar_current[0], fault_check_solar_current[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_voltage_true[command_len] = {fault_check_voltage[0], fault_check_voltage[1], 
                                                        true_arg[0], true_arg[1], true_arg[2], true_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]}; 

        constexpr int fault_check_voltage_false[command_len] = {fault_check_voltage[0], fault_check_voltage[1], 
                                                        false_arg[0], false_arg[1], false_arg[2], false_arg[3],
                                                        no_arg_2[0],  no_arg_2[1],  no_arg_2[2],  no_arg_2[3]};

        constexpr int known_commands[num_commands][command_len] = 
        {
            {mission_mode_low_power[0], mission_mode_low_power[1], mission_mode_low_power[2], mission_mode_low_power[3], mission_mode_low_power[4], mission_mode_low_power[5], mission_mode_low_power[6], mission_mode_low_power[7], mission_mode_low_power[8], mission_mode_low_power[9]},
            {mission_mode_deployment[0], mission_mode_deployment[1], mission_mode_deployment[2], mission_mode_deployment[3], mission_mode_deployment[4], mission_mode_deployment[5],mission_mode_deployment[6], mission_mode_deployment[7], mission_mode_deployment[8], mission_mode_deployment[9]},
            {mission_mode_standby[0], mission_mode_standby[1], mission_mode_standby[2], mission_mode_standby[3], mission_mode_standby[4], mission_mode_standby[5], mission_mode_standby[6], mission_mode_standby[7], mission_mode_standby[8], mission_mode_standby[9]},
            {mission_mode_safe[0], mission_mode_safe[1], mission_mode_safe[2], mission_mode_safe[3], mission_mode_safe[4], mission_mode_safe[5], mission_mode_safe[6], mission_mode_safe[7], mission_mode_safe[8], mission_mode_safe[9]},
            {burnwire_arm_true[0], burnwire_arm_true[1], burnwire_arm_true[2], burnwire_arm_true[3], burnwire_arm_true[4], burnwire_arm_true[5], burnwire_arm_true[6], burnwire_arm_true[7], burnwire_arm_true[8], burnwire_arm_true[9]},
            {burnwire_arm_false[0], burnwire_arm_false[1], burnwire_arm_false[2], burnwire_arm_false[3], burnwire_arm_false[4], burnwire_arm_false[5], burnwire_arm_false[6], burnwire_arm_false[7], burnwire_arm_false[8], burnwire_arm_false[9]},
            {burnwire_fire_true[0], burnwire_fire_true[1], burnwire_fire_true[2], burnwire_fire_true[3], burnwire_fire_true[4], burnwire_fire_true[5], burnwire_fire_true[6], burnwire_fire_true[7], burnwire_fire_true[8], burnwire_fire_true[9]},
            {burnwire_fire_false[0], burnwire_fire_false[1], burnwire_fire_false[2], burnwire_fire_false[3], burnwire_fire_false[4], burnwire_fire_false[5], burnwire_fire_false[6], burnwire_fire_false[7], burnwire_fire_false[8], burnwire_fire_false[9]},
            {camera_take_photo_true[0], camera_take_photo_true[1], camera_take_photo_true[2], camera_take_photo_true[3], camera_take_photo_true[4], camera_take_photo_true[5], camera_take_photo_true[6], camera_take_photo_true[7], camera_take_photo_true[8], camera_take_photo_true[9]},
            {camera_take_photo_false[0], camera_take_photo_false[1], camera_take_photo_false[2], camera_take_photo_false[3], camera_take_photo_false[4], camera_take_photo_false[5], camera_take_photo_false[6], camera_take_photo_false[7], camera_take_photo_false[8], camera_take_photo_false[9]},
            {temperature_mode_active[0], temperature_mode_active[1], temperature_mode_active[2], temperature_mode_active[3], temperature_mode_active[4], temperature_mode_active[5], temperature_mode_active[6], temperature_mode_active[7], temperature_mode_active[8], temperature_mode_active[9]},
            {temperature_mode_inactive[0], temperature_mode_inactive[1], temperature_mode_inactive[2], temperature_mode_inactive[3], temperature_mode_inactive[4], temperature_mode_inactive[5], temperature_mode_inactive[6], temperature_mode_inactive[7], temperature_mode_inactive[8], temperature_mode_inactive[9]},
            {acs_mode_detumbling[0], acs_mode_detumbling[1], acs_mode_detumbling[2], acs_mode_detumbling[3], acs_mode_detumbling[4], acs_mode_detumbling[5], acs_mode_detumbling[6], acs_mode_detumbling[7], acs_mode_detumbling[8], acs_mode_detumbling[9]},
            {acs_mode_pointing[0], acs_mode_pointing[1], acs_mode_pointing[2], acs_mode_pointing[3], acs_mode_pointing[4], acs_mode_pointing[5], acs_mode_pointing[6], acs_mode_pointing[7], acs_mode_pointing[8], acs_mode_pointing[9]},
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
            {fault_check_voltage_false[0], fault_check_voltage_false[1], fault_check_voltage_false[2], fault_check_voltage_false[3], fault_check_voltage_false[4], fault_check_voltage_false[5], fault_check_voltage_false[6], fault_check_voltage_false[7], fault_check_voltage_false[8], fault_check_voltage_false[9]}
        };
    }
    namespace temperature
    {
        constexpr int pin = 16;
        constexpr float min_temp_c;
        constexpr float max_temp_c;
    }
    namespace current
    {
        constexpr int pin = 15;
        constexpr float max_solar_current;
        constexpr float min_solar_current;
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
        constexpr int max_voltage;
        constexpr int min_voltage;
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

        //fault 2
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

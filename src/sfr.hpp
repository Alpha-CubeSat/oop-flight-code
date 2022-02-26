#ifndef SFR_HPP_
#define SFR_HPP_

#include "Arduino.h"
#include "Control Tasks/BurnwireControlTask.hpp"
#include "Control Tasks/TimedControlTask.hpp"
#include "MissionManager.hpp"
#include "Modes/acs_mode_type.enum"
#include "Modes/burnwire_mode_type.enum"
#include "Modes/camera_init_mode_type.enum"
#include "Modes/fault_index_type.enum"
#include "Modes/fault_mode_type.enum"
#include "Modes/imu_downlink_type.enum"
#include "Modes/mission_mode_type.enum"
#include "Modes/rockblock_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "Modes/simple_acs_type.enum"
#include "Pins.hpp"
#include "RockblockSimulator.hpp"
#include "constants.hpp"
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_VC0706.h>
#include <SD.h>
#include <StarshotACS0.h>
#include <cmath>
#include <deque>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>

namespace sfr {
    namespace pins {
        extern std::map<int, int> pinMap;
    } // namespace pins
    namespace photoresistor {
        extern bool covered;
    } // namespace photoresistor
    namespace mission {
        extern mission_mode_type mode;
        extern bool low_power_eligible;
    } // namespace mission
    namespace burnwire {
        extern bool fire;
        extern bool arm;
        extern burnwire_mode_type mode;
        extern int attempts;
        extern int start_time;
        extern int camera_attempts;
        extern int burn_time;
        extern int armed_time;
    } // namespace burnwire
    namespace camera {
        extern sensor_mode_type mode;
        extern bool photo_taken_sd_failed;
        extern bool take_photo;
        extern bool turn_on;
        extern bool turn_off;
        extern bool powered;

        extern camera_init_mode_type init_mode;
        extern uint8_t start_progress;
        extern uint64_t step_time;
        extern uint64_t init_start_time;
        extern uint64_t init_timeout;
        extern uint8_t begin_delay;
        extern uint8_t resolution_set_delay;
        extern uint8_t resolution_get_delay;

        extern uint64_t buffer[255];
        extern int current_serial;
        extern int fragment_number;
        extern int fragment_number_requested;
        extern int serial_requested;
        extern bool fragment_requested;
        extern int images_written;
        extern int fragments_written;
        extern int image_lengths[255];
        extern bool report_ready;
        extern bool full_image_written;
        extern bool report_downlinked;
        extern char filename[15];
        extern uint16_t jpglen;
        extern uint8_t set_res;
    } // namespace camera
    namespace rockblock {
        extern unsigned long last_communication;
        extern bool last_downlink_normal;
        extern int camera_commands[99][constants::rockblock::command_len];
        extern int camera_max_fragments[99];
        extern bool downlink_camera;
        extern unsigned long last_downlink;
        extern unsigned long downlink_period;
        extern unsigned long camera_downlink_period;
        extern rockblock_mode_type mode;
        extern bool waiting_message;
        extern char buffer[constants::rockblock::buffer_size];
        extern uint8_t report[constants::rockblock::packet_size];
        extern uint8_t camera_report[constants::rockblock::packet_size];
        extern int commas[constants::rockblock::num_commas];
        extern uint8_t opcode[2];
        extern uint8_t arg_1[4];
        extern uint8_t arg_2[4];
#ifndef SIMULATOR
        extern HardwareSerial serial;
#else
        extern RockblockSimulator serial;
#endif
        extern bool flush_status;
        extern bool waiting_command;
        extern size_t conseq_reads;
        extern uint16_t f_opcode;
        extern uint32_t f_arg_1;
        extern uint32_t f_arg_2;
        extern int timeout;
        extern int start_time;
        extern bool last_timed_out;
        extern int num_downlinks;
    } // namespace rockblock
    namespace imu {
        extern sensor_mode_type mode;

        extern float mag_x;
        extern float mag_y;
        extern float mag_z;
        extern float gyro_x;
        extern float gyro_y;
        extern float gyro_z;

        extern std::deque<float> mag_x_buffer;
        extern std::deque<float> mag_y_buffer;
        extern std::deque<float> mag_z_buffer;
        extern std::deque<float> gyro_x_buffer;
        extern std::deque<float> gyro_y_buffer;
        extern std::deque<float> gyro_z_buffer;
        // std::deque<std::experimental::any, time_t> imu_dlink_buffer;
        extern std::deque<float> imu_dlink_gyro_x_buffer;
        extern std::deque<float> imu_dlink_gyro_y_buffer;
        extern std::deque<float> imu_dlink_gyro_z_buffer;
        extern std::deque<time_t> imu_dlink_time_buffer;
        extern std::deque<imu_downlink_type> imu_dlink_magid_buffer;

        extern float mag_x_average;
        extern float mag_y_average;
        extern float mag_z_average;
        extern float gyro_x_average;
        extern float gyro_y_average;
        extern float gyro_z_average;

        extern bool imu_dlink_report_ready;
        extern imu_downlink_type imu_dlink_magid;
        extern const int imu_downlink_buffer_max_size;
        extern const int imu_downlink_report_size;
        extern uint8_t report[];

        extern const int mag_8GAUSS_min;
        extern const int mag_12GAUSS_min;
        extern const int mag_16GAUSS_min;
        extern const int gyro_500DPS_min;
        extern const int gyro_2000DPS_min;
    } // namespace imu
    namespace temperature {
        extern float temp_c;
        extern std::deque<float> temp_c_buffer;
        extern float temp_c_average;
        extern bool in_sun;
    } // namespace temperature
    namespace current {
        extern float solar_current;
        extern std::deque<float> solar_current_buffer;
        extern float solar_current_average;
        extern bool in_sun;
    } // namespace current
    namespace acs {
        extern acs_mode_type mode;
        extern float current1;
        extern float current2;
        extern float current3;
        extern simple_acs_type mag;
        extern unsigned long max_no_communication;
    } // namespace acs
    namespace battery {
        extern float voltage;
        extern std::deque<float> voltage_buffer;
        extern float voltage_average;
    } // namespace battery
    namespace fault {
        extern fault_mode_type mode;

        extern unsigned char fault_1;
        extern unsigned char fault_2;
        extern unsigned char fault_3;

        // FAULT 1
        extern bool check_mag_x;
        extern bool check_mag_y;
        extern bool check_mag_z;
        extern bool check_gyro_x;
        extern bool check_gyro_y;
        extern bool check_gyro_z;
        extern bool check_acc_x;
        extern bool check_acc_y;

        // FAULT 2
        extern bool check_acc_z;
        extern bool check_temp_c;
        extern bool check_solar_current;
        extern bool check_voltage;

        // FAULT 3
        extern bool check_burn_wire;
        extern bool check_sd_card;
        extern bool check_camera_on_failed;
    } // namespace fault
    namespace button {
        extern bool pressed;
    }
}; // namespace sfr

#endif
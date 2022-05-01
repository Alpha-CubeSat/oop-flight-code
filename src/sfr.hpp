#ifndef SFR_HPP_
#define SFR_HPP_

#include "Arduino.h"
#include "Control Tasks/BurnwireControlTask.hpp"
#include "Control Tasks/TimedControlTask.hpp"
#include "MissionManager.hpp"
#include "MissionMode.hpp"
#include "Modes/acs_mode_type.enum"
#include "Modes/burnwire_mode_type.enum"
#include "Modes/camera_init_mode_type.enum"
#include "Modes/fault_index_type.enum"
#include "Modes/fault_mode_type.enum"
#include "Modes/imu_downlink_type.enum"
#include "Modes/report_type.enum"
#include "Modes/rockblock_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "Modes/simple_acs_type.enum"
#include "Pins.hpp"
#include "RockblockSimulator.hpp"
#include "SensorReading.hpp"
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
namespace detumble {
    extern float start_time;
    extern float max_time;
    extern float stable_gyro_z;
} // namespace detumble
namespace aliveSignal {
    extern int num_downlink_failures;
    extern int max_downlink_failures;
    extern bool downlinked;
} // namespace aliveSignal
namespace pins {
    extern std::map<int, int> pinMap;
} // namespace pins
namespace photoresistor {
    extern int val;
    extern bool covered;
} // namespace photoresistor
namespace mission {
    extern MissionMode* boot;
    extern MissionMode* aliveSignal;
    extern MissionMode* lowPowerAliveSignal;
    extern MissionMode* detumbleSpin;
    extern MissionMode* lowPowerDetumbleSpin;
    extern MissionMode* normal;
    extern MissionMode* transmit;
    extern MissionMode* lowPower;
    extern MissionMode* normalDeployment;
    extern MissionMode* transmitDeployment;
    extern MissionMode* lowPowerDeployment;
    extern MissionMode* normalArmed;
    extern MissionMode* transmitArmed;
    extern MissionMode* lowPowerArmed;
    extern MissionMode* normalInSun;
    extern MissionMode* transmitInSun;
    extern MissionMode* lowPowerInSun;
    extern MissionMode* voltageFailureInSun;
    extern MissionMode* bootCamera;
    extern MissionMode* mandatoryBurns;
    extern MissionMode* regularBurns;
    extern MissionMode* photo;

    extern MissionMode* current_mode;
    extern MissionMode* previous_mode;
    extern unsigned long boot_start;
    extern unsigned long max_boot_time;
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
    // Report Types
    extern report_type downlink_report_type;
    extern bool rockblock_ready_status;
    extern rockblock_mode_type mode;

    // Time Parameters
    extern unsigned long last_communication;
    extern unsigned long last_downlink;
    extern unsigned long downlink_period;

    extern bool waiting_message;

    // Report Data
    extern std::deque<uint8_t> downlink_report;
    extern std::deque<uint8_t> normal_report;
    extern std::deque<uint8_t> camera_report;
    extern uint8_t imu_report[constants::rockblock::packet_size];

    extern char buffer[constants::rockblock::buffer_size];
    extern int camera_commands[99][constants::rockblock::command_len];
    extern int camera_max_fragments[99];
    extern int commas[constants::rockblock::num_commas];

    extern uint8_t opcode[2];
    extern uint8_t arg_1[4];
    extern uint8_t arg_2[4];

    extern int imu_max_fragments;
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
    extern bool successful_init;

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
    extern std::deque<float> acc_x_buffer;
    extern std::deque<float> acc_y_buffer;
    extern std::deque<float> acc_z_buffer;
    // std::deque<std::experimental::any, time_t> imu_dlink_buffer;
    extern std::deque<float> imu_dlink_gyro_x_buffer;
    extern std::deque<float> imu_dlink_gyro_y_buffer;
    extern std::deque<float> imu_dlink_gyro_z_buffer;

    // extern float mag_x_average;
    extern SensorReading* mag_x_average;
    extern SensorReading* mag_y_average;
    extern SensorReading* mag_z_average;
    extern SensorReading* gyro_x_average;
    extern SensorReading* gyro_y_average;
    extern SensorReading* gyro_z_average;
    extern SensorReading* acc_x_average;
    extern SensorReading* acc_y_average;

    extern imu_downlink_type imu_dlink_magid;
    extern const int imu_downlink_buffer_max_size;
    // extern const int imu_downlink_report_size;

    extern int fragment_number;
    extern int fragment_number_requested;
    extern bool fragment_requested;
    extern int fragments_written;
    extern bool imu_dlink_report_ready;
    extern bool report_downlinked;
    extern char filename[15];

    extern const int mag_8GAUSS_min;
    extern const int mag_12GAUSS_min;
    extern const int mag_16GAUSS_min;
    extern const int gyro_500DPS_min;
    extern const int gyro_2000DPS_min;
} // namespace imu
namespace temperature {
    extern float temp_c;
    extern std::deque<float> temp_c_buffer;
    extern SensorReading* temp_c_average;
    extern bool in_sun;
} // namespace temperature
namespace current {
    extern float solar_current;
    extern std::deque<float> solar_current_buffer;
    extern SensorReading* solar_current_average;
    extern bool in_sun;
} // namespace current
namespace acs {
    extern acs_mode_type mode;
    extern float current1;
    extern float current2;
    extern float current3;
    extern float pwm1;
    extern float pwm2;
    extern float pwm3;
    extern simple_acs_type mag;
    extern unsigned long max_no_communication;
} // namespace acs
namespace battery {
    extern float voltage;
    extern std::deque<float> voltage_buffer;
    extern SensorReading* voltage_average;
    extern float acceptable_battery;
    extern float min_battery;
} // namespace battery
namespace fault {
    extern fault_mode_type mode;

    // extern unsigned char fault_1;
    // extern unsigned char fault_2;
    // extern unsigned char fault_3;

    // // FAULT 1
    // extern bool check_mag_x;
    // extern bool check_mag_y;
    // extern bool check_mag_z;
    // extern bool check_gyro_x;
    // extern bool check_gyro_y;
    // extern bool check_gyro_z;
    // extern bool check_acc_x;
    // extern bool check_acc_y;

    // // FAULT 2
    // extern bool check_acc_z;
    // extern bool check_temp_c;
    // extern bool check_solar_current;
    // extern bool check_voltage;

    // // FAULT 3
    // extern bool check_burn_wire;
    // extern bool check_sd_card;
    // extern bool check_camera_on_failed;
} // namespace fault
namespace button {
    extern bool pressed;
}
}; // namespace sfr

#endif
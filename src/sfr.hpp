#ifndef SFR_HPP_
#define SFR_HPP_

#include "Arduino.h"
#include "Control Tasks/BurnwireControlTask.hpp"
#include "Control Tasks/EEPROMControlTask.hpp"
#include "Control Tasks/TimedControlTask.hpp"
#include "MissionManager.hpp"
#include "MissionMode.hpp"
#include "Modes/burnwire_mode_type.enum"
#include "Modes/camera_init_mode_type.enum"
#include "Modes/fault_index_type.enum"
#include "Modes/fault_mode_type.enum"
#include "Modes/imu_downlink_type.enum"
#include "Modes/mode_type.enum"
#include "Modes/report_type.enum"
#include "Modes/rockblock_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "Modes/simple_acs_type.enum"
#include "Phase.hpp"
#include "Pins.hpp"
#include "RockblockCommand.hpp"
#include "RockblockSimulator.hpp"
#include "SFRField.hpp"
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
#include <queue>
#include <sstream>
#include <string>

namespace sfr {
    namespace stabilization {
        extern SFRField<uint32_t> max_time;
    } // namespace stabilization
    namespace boot {
        extern SFRField<uint32_t> max_time;
    }
    namespace simple {
        extern SFRField<uint32_t> max_time;
    }
    namespace point {
        extern SFRField<uint32_t> max_time;
    }
    namespace detumble {
        extern SFRField<uint32_t> start_time;
        extern SFRField<uint32_t> max_time;
        extern SFRField<uint16_t> num_imu_retries;
        extern SFRField<uint16_t> max_imu_retries;
        extern float min_stable_gyro_z;
        extern float max_stable_gyro_x;
        extern float max_stable_gyro_y;
        extern float min_unstable_gyro_x;
        extern float min_unstable_gyro_y;
    } // namespace detumble
    namespace aliveSignal {
        extern SFRField<uint16_t> max_downlink_hard_faults;
        extern SFRField<bool> downlinked;
        extern SFRField<uint32_t> max_time;
        extern SFRField<uint16_t> num_hard_faults;
    } // namespace aliveSignal
    namespace pins {
        extern std::map<int, int> pinMap;
    } // namespace pins
    namespace photoresistor {
        extern SFRField<bool> covered;
    } // namespace photoresistor
    namespace mission {
        extern MissionMode *boot;
        extern MissionMode *aliveSignal;
        extern MissionMode *lowPowerAliveSignal;
        extern MissionMode *detumbleSpin;
        extern MissionMode *lowPowerDetumbleSpin;
        extern MissionMode *normal;
        extern MissionMode *transmit;
        extern MissionMode *lowPower;
        extern MissionMode *normalDeployment;
        extern MissionMode *transmitDeployment;
        extern MissionMode *lowPowerDeployment;
        extern MissionMode *normalArmed;
        extern MissionMode *transmitArmed;
        extern MissionMode *lowPowerArmed;
        extern MissionMode *normalInSun;
        extern MissionMode *transmitInSun;
        extern MissionMode *lowPowerInSun;
        extern MissionMode *voltageFailureInSun;
        extern MissionMode *bootCamera;
        extern MissionMode *mandatoryBurns;
        extern MissionMode *regularBurns;
        extern MissionMode *photo;

        extern Phase *initialization;
        extern Phase *stabilization;
        extern Phase *standby;
        extern Phase *deployment;
        extern Phase *armed;
        extern Phase *inSun;
        extern Phase *firing;

        extern MissionMode *current_mode;
        extern MissionMode *previous_mode;

        extern SFRField<uint32_t> time_deployed;
        extern SFRField<bool> deployed;
        extern SFRField<bool> already_deployed;

        extern Phase *current_phase;
        extern Phase *previous_phase;

        extern std::deque<int> mode_history;

        extern SFRField<uint32_t> acs_transmit_cycle_time;
    } // namespace mission
    namespace burnwire {
        extern burnwire_mode_type mode;
        extern SFRField<bool> fire;
        extern SFRField<bool> arm;
        extern SFRField<uint16_t> attempts;
        extern SFRField<uint16_t> camera_attempts;
        extern SFRField<uint32_t> start_time;
        extern SFRField<uint32_t> burn_time;
        extern SFRField<uint32_t> armed_time;
    } // namespace burnwire
    namespace camera {
        extern sensor_mode_type mode;
        extern SFRField<bool> photo_taken_sd_failed;
        extern SFRField<bool> take_photo;
        extern SFRField<bool> turn_on;
        extern SFRField<bool> turn_off;
        extern SFRField<bool> powered;

        extern camera_init_mode_type init_mode;
        extern SFRField<uint8_t> start_progress;
        extern SFRField<uint32_t> step_time;
        extern SFRField<uint32_t> init_start_time;
        extern SFRField<uint32_t> init_timeout;
        extern SFRField<uint32_t> begin_delay;
        extern SFRField<uint32_t> resolution_set_delay;
        extern SFRField<uint32_t> resolution_get_delay;

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
        extern SFRField<bool> rockblock_ready_status;
        extern rockblock_mode_type mode;

        // Time Parameters
        extern SFRField<uint32_t> last_downlink;
        extern SFRField<uint32_t> downlink_period;

        extern SFRField<bool> waiting_message;

        // Report Data
        extern std::deque<uint8_t> downlink_report;
        extern std::deque<uint8_t> normal_report;
        extern std::deque<uint8_t> camera_report;
        extern std::deque<uint8_t> imu_report;

        extern char buffer[constants::rockblock::buffer_size];
        extern int camera_commands[99][constants::rockblock::command_len];
        extern int camera_max_fragments[99];
        extern int commas[constants::rockblock::num_commas];

        extern std::deque<RawRockblockCommand> raw_commands;
        extern std::deque<RockblockCommand> processed_commands;
        extern SFRField<uint8_t> max_commands_count;

        extern int imu_downlink_max_fragments[99];
        extern SFRField<uint16_t> imu_max_fragments;

        extern SFRField<uint32_t> imudownlink_start_time;
        extern SFRField<uint32_t> imudownlink_remain_time;
        extern SFRField<bool> imu_first_start;
        extern SFRField<bool> imu_downlink_on;
#ifndef SIMULATOR
        extern HardwareSerial serial;
#else
        extern RockblockSimulator serial;
#endif
        extern SFRField<bool> flush_status;
        extern SFRField<bool> waiting_command;
        extern SFRField<uint32_t> conseq_reads;
        extern SFRField<uint32_t> start_time_check_signal;
        extern SFRField<uint32_t> max_check_signal_time;
        extern SFRField<bool> sleep_mode;
    } // namespace rockblock
    namespace imu {
        extern sensor_mode_type mode;
        extern SFRField<bool> successful_init;

        extern std::deque<uint8_t> imu_dlink;
        extern imu_downlink_type imu_dlink_mode;

        extern bool sample;
        extern uint8_t fragment_number;
        extern uint8_t current_sample;
        extern bool sample_gyro;
        extern int gyro_min;
        extern int gyro_max;
        extern int mag_min;
        extern int mag_max;
        extern int acc_min;
        extern int acc_max;
        extern bool report_ready;
        extern bool report_downlinked;
        extern bool report_written;
        extern bool full_report_written;
        extern int max_fragments;
        extern int content_length;
    } // namespace imu
    namespace temperature {
        extern SFRField<bool> in_sun;
    } // namespace temperature
    namespace current {
        extern SFRField<bool> in_sun;
    } // namespace current
    namespace acs {
        extern float current1;
        extern float current2;
        extern float current3;
        extern float pwm1;
        extern float pwm2;
        extern float pwm3;
        extern simple_acs_type mag;
        extern SFRField<uint32_t> max_no_communication;
        extern SFRField<uint32_t> on_time;

        extern SFRField<bool> off;
    } // namespace acs
    namespace battery {
        extern uint32_t acceptable_battery;
        extern uint32_t min_battery;
    } // namespace battery
    namespace fault {
        extern fault_mode_type mode;
    } // namespace fault
    namespace button {
        extern SFRField<bool> pressed;
    }
    namespace EEPROM {
        extern SFRField<uint32_t> time_of_last_write;
        extern SFRField<uint32_t> write_step_time;
        extern SFRField<uint32_t> alloted_time;
        extern SFRField<uint32_t> eeprom_value;
        extern SFRField<bool> alloted_time_passed;
    } // namespace EEPROM
};    // namespace sfr

#endif
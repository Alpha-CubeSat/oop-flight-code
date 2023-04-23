#ifndef SFR_HPP_
#define SFR_HPP_

#include "Adafruit_VC0706.h"
#include "Arduino.h"
#include "MissionMode.hpp"
#include "Modes/burnwire_mode_type.enum"
#include "Modes/report_type.enum"
#include "Modes/rockblock_mode_type.enum"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "Modes/simple_acs_type.enum"
#include "Phase.hpp"
#include "RockblockCommand.hpp"
#include "RockblockSimulator.hpp"
#include "SFRField.hpp"
#include "SensorReading.hpp"
#include "constants.hpp"
#include <deque>
#include <vector>

namespace sfr {
    namespace stabilization {
        // OP Codes 1100
        extern SFRField<uint32_t> max_time;
    } // namespace stabilization
    namespace boot {
        // OP Codes 1200
        extern SFRField<uint32_t> max_time;
    } // namespace boot
    namespace simple {
        // OP Codes 1300
        extern SFRField<uint32_t> max_time;
    } // namespace simple
    namespace point {
        // OP Codes 1400
        extern SFRField<uint32_t> max_time;
    } // namespace point
    namespace detumble {
        // OP Codes 1500
        extern SFRField<uint32_t> start_time;
        extern SFRField<uint32_t> max_time;

        extern SFRField<uint8_t> min_stable_gyro_z;
        extern SFRField<uint8_t> max_stable_gyro_x;
        extern SFRField<uint8_t> max_stable_gyro_y;

        extern SFRField<uint8_t> min_unstable_gyro_x;
        extern SFRField<uint8_t> min_unstable_gyro_y;
    } // namespace detumble
    namespace aliveSignal {
        // OP Codes 1600
        extern SFRField<uint16_t> max_downlink_hard_faults;
        extern SFRField<bool> downlinked;
        extern SFRField<uint32_t> max_time;
        extern SFRField<uint16_t> num_hard_faults;
    } // namespace aliveSignal
    namespace photoresistor {
        // OP Codes 1700
        extern SFRField<bool> covered;

        extern SensorReading *light_val_average_standby;
        extern SensorReading *light_val_average_deployment;

    } // namespace photoresistor
    namespace mission {
        // OP Codes 1800
        extern SFRField<uint32_t> acs_transmit_cycle_time;

        extern SFRField<uint32_t> time_deployed;
        extern SFRField<bool> deployed;
        extern SFRField<bool> already_deployed;
        extern SFRField<bool> possible_uncovered;

        extern Boot boot_class;
        extern AliveSignal aliveSignal_class;
        extern LowPowerAliveSignal lowPowerAliveSignal_class;
        extern DetumbleSpin detumbleSpin_class;
        extern LowPowerDetumbleSpin lowPowerDetumbleSpin_class;
        extern Normal normal_class;
        extern Transmit transmit_class;
        extern LowPower lowPower_class;
        extern NormalDeployment normalDeployment_class;
        extern TransmitDeployment transmitDeployment_class;
        extern LowPowerDeployment lowPowerDeployment_class;
        extern NormalArmed normalArmed_class;
        extern TransmitArmed transmitArmed_class;
        extern LowPowerArmed lowPowerArmed_class;
        extern NormalInSun normalInSun_class;
        extern TransmitInSun transmitInSun_class;
        extern LowPowerInSun lowPowerInSun_class;
        extern VoltageFailureInSun voltageFailureInSun_class;
        extern BootIMU bootImu_class;
        extern BootCamera bootCamera_class;
        extern MandatoryBurns mandatoryBurns_class;
        extern RegularBurns regularBurns_class;
        extern Photo photo_class;

        extern Initialization initialization_class;
        extern Stabilization stabilization_class;
        extern Standby standby_class;
        extern Deployment deployment_class;
        extern Armed armed_class;
        extern InSun insun_class;
        extern Firing firing_class;

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
        extern MissionMode *bootImu;
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

        extern Phase *current_phase;
        extern Phase *previous_phase;

        extern std::deque<int> mode_history;
    } // namespace mission
    namespace burnwire {
        // OP Codes 1900
        extern SFRField<uint16_t> attempts;
        extern SFRField<uint32_t> start_time;
        extern SFRField<uint32_t> burn_time;
        extern SFRField<uint32_t> armed_time;
        extern SFRField<uint16_t> mode;
        extern SFRField<uint16_t> attempts_limit;
        extern SFRField<uint16_t> mandatory_attempts_limit;
        extern SFRField<uint32_t> delay_time;
    } // namespace burnwire
    namespace camera {
        // OP Codes 2000
        extern SFRField<bool> photo_taken_sd_failed;
        extern SFRField<bool> take_photo;
        extern SFRField<bool> turn_on;
        extern SFRField<bool> turn_off;
        extern SFRField<bool> powered;

        // Initialization
        extern SFRField<uint8_t> start_progress;
        extern SFRField<uint32_t> step_time;
        extern SFRField<uint32_t> init_start_time;
        extern SFRField<uint32_t> init_timeout;
        extern SFRField<uint32_t> begin_delay;
        extern SFRField<uint32_t> resolution_set_delay;
        extern SFRField<uint32_t> resolution_get_delay;

        extern SFRField<uint16_t> init_mode;
        extern SFRField<uint16_t> mode;

        extern SFRField<uint32_t> images_written;
        extern SFRField<uint32_t> fragments_written;

        extern SFRField<uint32_t> set_res;

        extern SFRField<uint16_t> failed_times;
        extern SFRField<uint16_t> failed_limit;

        extern bool fragment_requested;
        extern SFRField<uint32_t> fragment_number_requested;
        extern SFRField<uint8_t> serial_requested;

        extern bool report_written;
        extern bool report_ready;

    } // namespace camera
    namespace rockblock {
        // OP Codes 2100
        extern SFRField<bool> ready_status;

        extern SFRField<uint32_t> last_downlink;
        extern SFRField<uint32_t> downlink_period;

        extern SFRField<bool> waiting_message;

        extern std::deque<uint8_t> downlink_report;
        extern std::deque<uint8_t> normal_report;
        extern std::deque<uint8_t> camera_report;
        extern std::deque<uint8_t> imu_report;

        extern uint8_t normal_report_command_curr;
        extern uint8_t normal_report_command_max;
        extern std::deque<uint8_t> commands_received;

        extern char buffer[constants::rockblock::buffer_size];
        extern int camera_commands[99][constants::rockblock::command_len];
        extern uint32_t camera_max_fragments[99];
        extern int commas[constants::rockblock::num_commas];

        extern std::deque<RawRockblockCommand> raw_commands;
        extern std::deque<RockblockCommand *> processed_commands;

        extern SFRField<uint8_t> max_commands_count;

        extern SFRField<uint16_t> imu_max_fragments;

        extern SFRField<uint32_t> imudownlink_start_time;
        extern SFRField<uint32_t> imudownlink_remain_time;
        extern SFRField<bool> imu_first_start;
        extern SFRField<bool> imu_downlink_on;

        extern SFRField<bool> flush_status;
        extern SFRField<bool> waiting_command;
        extern SFRField<uint32_t> conseq_reads;
        extern SFRField<uint32_t> timeout;
        extern SFRField<uint32_t> start_time;
        extern SFRField<uint32_t> start_time_check_signal;
        extern SFRField<uint32_t> max_check_signal_time;
        extern SFRField<bool> sleep_mode;

        extern SFRField<uint16_t> downlink_report_type;
        extern SFRField<uint16_t> mode;

#ifndef SIMULATOR
        extern HardwareSerial serial;
#else
        extern RockblockSimulator serial;
#endif

    } // namespace rockblock
    namespace imu {
        // OP Codes 2200
        extern SFRField<uint16_t> mode;

        extern SFRField<uint16_t> init_mode;

        extern SFRField<uint32_t> max_fragments;

        extern SFRField<bool> sample_gyro;

        extern SFRField<bool> turn_on;
        extern SFRField<bool> turn_off;
        extern SFRField<bool> powered;

        extern SFRField<uint16_t> failed_times;
        extern SFRField<uint16_t> failed_limit;

        extern SensorReading *mag_x_value;
        extern SensorReading *mag_y_value;
        extern SensorReading *mag_z_value;

        extern SensorReading *gyro_x_value;
        extern SensorReading *gyro_y_value;
        extern SensorReading *gyro_z_value;

        extern SensorReading *mag_x_average;
        extern SensorReading *mag_y_average;
        extern SensorReading *mag_z_average;

        extern SensorReading *gyro_x_average;
        extern SensorReading *gyro_y_average;
        extern SensorReading *gyro_z_average;

        extern SensorReading *acc_x_average;
        extern SensorReading *acc_y_average;

        extern std::deque<uint8_t> imu_dlink;

        extern SFRField<uint16_t> imu_boot_collection_start_time;
        extern SFRField<uint16_t> door_open__collection_start_time;

        extern bool report_written;
        extern bool report_ready;

    } // namespace imu
    namespace temperature {
        // OP Codes 2300
        extern SFRField<bool> in_sun;

        extern SensorReading *temp_c_average;
        extern SensorReading *temp_c_value;
    } // namespace temperature
    namespace current {
        // OP Codes 2400
        extern SFRField<bool> in_sun;

        extern SensorReading *solar_current_average;
    } // namespace current
    namespace acs {
        // OP Codes 2500
        extern SFRField<uint32_t> max_no_communication;

        extern SFRField<uint32_t> on_time;
        extern SFRField<bool> off;

        extern SFRField<uint16_t> mag;

        extern SFRField<uint32_t> detumble_timeout;
    } // namespace acs
    namespace battery {
        // OP Codes 2600
        // TODO
        extern SFRField<uint32_t> acceptable_battery;
        extern SFRField<uint32_t> min_battery;

        extern SensorReading *voltage_value;
        extern SensorReading *voltage_average;
    } // namespace battery
    namespace button {
        // OP Codes 2700
        extern SFRField<bool> pressed;

        extern SensorReading *button_pressed;
    } // namespace button
    namespace pins {
        // TODO confirm initial pin states are correct FS-159
        // @ Josh do we need to change this?
        extern std::map<int, int> pinMap;
    } // namespace pins
    namespace eeprom {
        // OP Codes 2800
        extern SFRField<uint8_t> boot_counter;
        extern int wait_time_last_write_time;
        extern SFRField<uint32_t> wait_time_write_step_time;
        extern SFRField<uint32_t> alloted_time;
        extern SFRField<bool> alloted_time_passed;
        extern int sfr_last_write_time;
        extern SFRField<uint32_t> sfr_write_step_time;
        extern uint16_t sfr_address;
        extern SFRField<uint32_t> sfr_address_age;
        extern SFRField<boolean> storage_full;
    } // namespace eeprom
};    // namespace sfr

#endif
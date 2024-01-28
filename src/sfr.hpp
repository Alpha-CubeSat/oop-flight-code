#ifndef SFR_HPP_
#define SFR_HPP_

#include "Adafruit_VC0706.h"
#include "Arduino.h"
#include "Faults.hpp"
#include "MissionMode.hpp"
#include "Modes/burnwire_mode_type.enum"
#include "Modes/mag_type.enum"
#include "Modes/report_type.enum"
#include "Modes/rockblock_mode_type.enum"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "Modes/sensor_power_mode_type.enum"
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
    namespace detumble {
        // OP Codes 1500
        extern SFRField<uint8_t> min_stable_gyro_z;
        extern SFRField<uint8_t> max_stable_gyro_x;
        extern SFRField<uint8_t> max_stable_gyro_y;
        extern SFRField<uint8_t> min_unstable_gyro_x;
        extern SFRField<uint8_t> min_unstable_gyro_y;
    } // namespace detumble
    namespace aliveSignal {
        // OP Codes 1600
        extern SFRField<bool> downlinked;
        extern SFRField<uint16_t> max_downlink_hard_faults;
        extern SFRField<uint16_t> num_hard_faults;
        extern SFRField<uint32_t> max_time;
    } // namespace aliveSignal
    namespace photoresistor {
        // OP Codes 1700
        extern SFRField<bool> covered;

        extern SensorReading *light_val_average_standby;
        extern SensorReading *light_val_average_deployment;
    } // namespace photoresistor
    namespace mission {
        // OP Codes 1800
        extern SFRField<bool> deployed;
        extern SFRField<bool> possible_uncovered;
        extern SFRField<uint8_t> boot_time_mins;
        extern SFRField<uint32_t> cycle_no;
        extern SFRField<uint32_t> cycle_start;

        extern Boot boot_class;
        extern AliveSignal aliveSignal_class;
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
        extern CaptureIMU captureIMU_class;
        extern BootSensors bootSensors_class;
        extern MandatoryBurns mandatoryBurns_class;
        extern RegularBurns regularBurns_class;
        extern DeploymentVerification deploymentVerification_class;

        extern Initialization initialization_class;
        extern Stabilization stabilization_class;
        extern Standby standby_class;
        extern Deployment deployment_class;
        extern Armed armed_class;
        extern InSun insun_class;
        extern Firing firing_class;

        extern MissionMode *boot;
        extern MissionMode *aliveSignal;
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
        extern MissionMode *captureIMU;
        extern MissionMode *bootSensors;
        extern MissionMode *mandatoryBurns;
        extern MissionMode *regularBurns;
        extern MissionMode *deploymentVerification;

        extern Phase *initialization;
        extern Phase *stabilization;
        extern Phase *standby;
        extern Phase *deployment;
        extern Phase *armed;
        extern Phase *inSun;
        extern Phase *firing;

        extern MissionMode *current_mode;
        extern MissionMode *previous_mode;

        extern Phase *previous_phase;

        extern std::deque<int> mode_history;
    } // namespace mission
    namespace burnwire {
        // OP Codes 1900
        extern SFRField<uint16_t> attempts;
        extern SFRField<uint16_t> mode;
        extern SFRField<uint16_t> attempts_limit;
        extern SFRField<uint16_t> mandatory_attempts_limit;
        extern SFRField<uint32_t> delay_time;
        extern SFRField<uint32_t> start_time;
        extern SFRField<uint32_t> burn_time;
        extern SFRField<uint32_t> armed_time;
    } // namespace burnwire
    namespace camera {
        // OP Codes 2000
        extern SFRField<bool> photo_taken_sd_failed;
        extern SFRField<bool> take_photo;
        extern SFRField<bool> powered;
        extern SFRField<bool> report_ready;
        extern SFRField<bool> fragment_requested;
        extern SFRField<uint8_t> serial_requested;
        extern SFRField<uint8_t> start_progress;
        extern SFRField<uint8_t> power_setting;
        extern SFRField<uint16_t> failed_times;
        extern SFRField<uint16_t> failed_limit;
        extern SFRField<uint16_t> init_mode;
        extern SFRField<uint16_t> mode;
        extern SFRField<uint32_t> init_start_time;
        extern SFRField<uint32_t> init_timeout;
        extern SFRField<uint32_t> images_written;
        extern SFRField<uint32_t> fragments_written;
        extern SFRField<uint32_t> set_res;
        extern SFRField<uint32_t> fragment_number_requested;
    } // namespace camera
    namespace rockblock {
        // OP Codes 2100
        extern SFRField<bool> ready_status;
        extern SFRField<bool> flush_status;
        extern SFRField<bool> waiting_command;
        extern SFRField<bool> sleep_mode;
        extern SFRField<uint8_t> max_commands_count;
        extern SFRField<uint16_t> downlink_report_type;
        extern SFRField<uint16_t> mode;
        extern SFRField<uint32_t> last_downlink;
        extern SFRField<uint32_t> downlink_period;
        extern SFRField<uint32_t> lp_downlink_period;
        extern SFRField<uint32_t> transmit_downlink_period;
        extern SFRField<uint32_t> on_time;

        extern char buffer[constants::rockblock::buffer_size];
        extern int camera_commands[99][constants::rockblock::command_len];
        extern uint8_t commas[constants::rockblock::num_commas];
        extern uint32_t camera_max_fragments[99];
        extern std::deque<uint16_t> commands_received;
        extern std::deque<RockblockCommand *> processed_commands;
        extern std::deque<uint8_t> downlink_report;
        extern std::deque<uint8_t> normal_report;
        extern std::deque<uint8_t> camera_report;
        extern std::deque<uint8_t> imu_report;

#ifndef SIMULATOR
        extern HardwareSerial serial;
#else
        extern RockblockSimulator serial;
#endif
    } // namespace rockblock
    namespace imu {
        // OP Codes 2200
        extern SFRField<bool> powered;
        extern SFRField<bool> report_written;
        extern SFRField<bool> report_ready;
        extern SFRField<uint8_t> power_setting;
        extern SFRField<uint16_t> mode;
        extern SFRField<uint16_t> init_mode;
        extern SFRField<uint16_t> failed_times;
        extern SFRField<uint16_t> failed_limit;
        extern SFRField<uint32_t> max_fragments;

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

        extern std::deque<uint8_t> imu_dlink;
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
        extern SFRField<bool> off;
        extern SFRField<uint8_t> mode;
        extern SFRField<uint8_t> simple_mag;
        extern SFRField<uint32_t> simple_current;
        extern SFRField<uint32_t> on_time;
        extern SFRField<uint32_t> Id_index;
        extern SFRField<uint32_t> Kd_index;
        extern SFRField<uint32_t> Kp_index;
        extern SFRField<uint32_t> c_index;
    } // namespace acs
    namespace battery {
        // OP Codes 2600
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
        extern std::map<int, int> pinMap;
    } // namespace pins
    namespace eeprom {
        // OP Codes 2800
        extern SFRField<bool> boot_mode;
        extern SFRField<bool> boot_restarted;
        extern SFRField<bool> error_mode;
        extern SFRField<bool> light_switch;
        extern SFRField<bool> sfr_save_completed;
        extern SFRField<uint8_t> boot_counter;
        extern SFRField<uint16_t> dynamic_data_addr;
        extern SFRField<uint16_t> sfr_data_addr;
        extern SFRField<uint32_t> time_alive;
        extern SFRField<uint32_t> dynamic_data_age;
        extern SFRField<uint32_t> sfr_data_age;
    } // namespace eeprom
};    // namespace sfr

#endif
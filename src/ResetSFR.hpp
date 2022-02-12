#ifndef PINS_HPP_
#define PINS_HPP_

#include "sfr.hpp"

class ResetSFR
{
public:
  static void capture_default_sfr();
  static void restore_default_sfr();

private:
  // need a pinMap reset? If yes, we can just call PinControlTask::execute().
  // pins are being updated in the switch cases in each control task anyway
  static bool photoresistor_covered;

  static mission_mode_type mission_mode;
  static bool mission_low_power_eligible;

  static bool burnwire_fire;
  static bool burnwire_arm;
  static burnwire_mode_type burnwire_mode;
  static int burnwire_attempts;
  static int burnwire_start_time;
  static int burnwire_burn_time;
  static int burnwire_armed_time;

  static bool camera_photo_take_sd_failed;
  static bool camera_take_photo;
  static bool camera_turn_on;
  static bool camera_turn_off;
  static bool camera_powered;
  static uint8_t camera_buffer[25];
  static int camera_current_serial;
  static int camera_fragment_number;
  static int camera_fragment_number_requested;
  static int camera_serial_requested;
  static bool camera_fragment_requested;
  static int camera_images_written;
  static int camera_fragments_written;
  static bool camera_report_ready;
  static bool camera_report_downlinked;
  static uint16_t camera_jpglen;

  static unsigned long rockblock_last_communication;
  static bool rockblock_last_downlink_normal;
  static int rockblock_camera_commands[99][constants::rockblock::command_len];
  static int rockblock_max_fragments[99];
  static bool rockblock_downlink_camera;
  static unsigned long rockblock_last_downlink;
  static unsigned long rockblock_downlink_period;
  static unsigned long rockblock_camera_downlinked_period;
  static rockblock_mode_type rockblock_mode;
  static bool rockblock_waiting_message;
  static char rockblock_buffer[constants::rockblock::buffer_size];
  static uint8_t rockblock_report[constants::rockblock::packet_size];
  static uint8_t rockblock_camera_report[constants::rockblock::packet_size];
  static int rockblock_commas[constants::rockblock::num_commas];
  static uint8_t rockblock_opcode[2];
  static uint8_t rockblock_arg_1[4];
  static uint8_t rockblock_arg_2[4];
  static HardwareSerial rockblock_hardware_serial;
  static RockblockSimulator rockblock_rs_serial;
  static bool rockblock_flush_status;
  static bool rockblock_waiting_command;
  static size_t rockblock_conseq_reads;
  static uint16_t rockblock_f_opcode;
  static uint32_t rockblock_f_arg_1;
  static uint32_t rockblock_f_arg_2;
  static int rockblock_timeout;
  static int rockblock_start_time;
  static bool rockblock_last_timed_out;
  static int rockblock_num_downlinks;

  static sensor_mode_type imu_mode;
  static int imu_max_retry_attempts;
  static float imu_mag_x;
  static float imu_mag_y;
  static float imu_mag_z;
  static float imu_gyro_x;
  static float imu_gyro_y;
  static float imu_gyro_z;
  static std::deque<float> imu_mag_x_buffer;
  static std::deque<float> imu_mag_y_buffer;
  static std::deque<float> imu_mag_z_buffer;
  static std::deque<float> imu_gyro_x_buffer;
  static std::deque<float> imu_gyro_y_buffer;
  static std::deque<float> imu_gyro_z_buffer;
  static float imu_mag_x_average;
  static float imu_mag_y_average;
  static float imu_mag_z_average;
  static float gyro_x_average;
  static float gyro_y_average;
  static float gyro_z_average;

  static float temperature_temp_c;
  static std::deque<float> temperature_temp_c_buffer;
  static float temperature_temp_c_average;
  static temp_mode_type temperature_mode;
  static bool temperature_in_sun;

  static float current_solar_current;
  static std::deque<float> current_solar_current_buffer;
  static float current_solar_current_average;
  static bool current_in_sun;

  static acs_mode_type acs_mode;
  static float acs_current1;
  static float acs_current2;
  static float acs_current3;
  static simple_acs_type acs_mag;
  static unsigned long acs_max_no_communication;

  static float battery_voltage;
  static std::deque<float> battery_voltage_buffer;
  static float battery_voltage_average;

  static fault_mode_type fault_mode;
  static unsigned char fault_fault_1;
  static unsigned char fault_fault_2;
  static unsigned char fault_fault_3;
};

#endif
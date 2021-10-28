#ifndef PINS_HPP_
#define PINS_HPP_

#include "sfr.hpp"

class ResetSFR
{
public:
  static void capture_default_sfr();
  static void restore_default_sfr();
private:
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




};

#endif
#ifndef SFR_HPP_
#define SFR_HPP_

#include "Adafruit_VC0706.h"
#include "Arduino.h"
#include "MissionMode.hpp"
#include "Modes/burnwire_mode_type.enum"
#include "Modes/camera_init_mode_type.enum"
#include "Modes/report_type.enum"
#include "Modes/rockblock_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "Modes/simple_acs_type.enum"
#include "Phase.hpp"
#include "SFRField.hpp"
#include "SensorReading.hpp"
#include "constants.hpp"

namespace sfr {
    namespace stabilization {
        // OP Codes 1100
        SFRField<uint32_t> max_time(30 * constants::time::one_minute, 0UL, 5 * constants::time::one_hour, 1100);
    } // namespace stabilization
    namespace boot {
        // OP Codes 1200
        SFRField<uint32_t> max_time(2 * constants::time::one_hour, 10UL, 5 * constants::time::one_hour, 1200);
    } // namespace boot
    namespace simple {
        // OP Codes 1300
        SFRField<uint32_t> max_time(5 * constants::time::one_minute, 10UL, 5 * constants::time::one_hour, 1300);
    } // namespace simple
    namespace point {
        // OP Codes 1400
        SFRField<uint32_t> max_time(5 * constants::time::one_minute, 1400);
    } // namespace point
    namespace detumble {
        // OP Codes 1500
        SFRField<uint32_t> start_time(0UL, 1500);
        SFRField<uint32_t> max_time(2 * constants::time::one_hour, 1501);
        // TODO
        SFRField<uint16_t> num_imu_retries(0, 1502);
        SFRField<uint16_t> max_imu_retries(5, 1503);
    } // namespace detumble
    namespace aliveSignal {
        // OP Codes 1600
        SFRField<uint16_t> max_downlink_hard_faults(3, 1600);
        SFRField<bool> downlinked(false, 1601);
        SFRField<uint32_t> max_time(2 * constants::time::one_hour, 1602);
        SFRField<uint16_t> num_hard_faults(0, 1603);
    } // namespace aliveSignal
    namespace pins {
        //@Josh would we need to change this?
        std::map<int, int> pinMap = {
            {constants::photoresistor::pin, LOW},
            {constants::burnwire::first_pin, LOW},
            {constants::burnwire::second_pin, LOW},
            {constants::rockblock::sleep_pin, LOW},
            {constants::temperature::pin, LOW},
            {constants::current::pin, LOW},
            {constants::acs::xPWMpin, LOW},
            {constants::acs::yPWMpin, LOW},
            {constants::acs::zPWMpin, LOW},
            {constants::acs::yout1, LOW},
            {constants::acs::yout2, LOW},
            {constants::acs::xout1, LOW},
            {constants::acs::xout2, LOW},
            {constants::acs::zout1, LOW},
            {constants::acs::zout2, LOW},
            {constants::acs::STBXYpin, LOW},
            {constants::acs::STBZpin, LOW},
            {constants::battery::voltage_value_pin, LOW},
            {constants::battery::allow_measurement_pin, HIGH},
            {constants::camera::power_on_pin, LOW},
            {constants::camera::rx, LOW},
            {constants::camera::tx, LOW},
            {constants::button::button_pin, LOW}};
    } // namespace pins
    namespace photoresistor {
        // OP Codes 1700
        SFRField<bool> covered(true, 1700);
    } // namespace photoresistor
    namespace mission {
        // OP Codes 1800
        SFRField<uint32_t> acs_transmit_cycle_time(constants::time::one_minute * 100, 1800);

        SFRField<uint32_t> time_deployed(0, 1801);
        SFRField<bool> deployed(false, 1802);
        SFRField<bool> already_deployed(false, 1803);
    } // namespace mission
    namespace burnwire {
        // OP Codes 1900
        SFRField<bool> fire(false, 1900);
        SFRField<bool> arm(false, 1901);
        SFRField<uint16_t> attempts(0, 1902);
        SFRField<uint16_t> camera_attempts(0, 1904);
        SFRField<uint32_t> start_time(0, 1903);
        SFRField<uint32_t> burn_time(500, 1905);
        SFRField<uint32_t> armed_time(2 * constants::time::one_day, 1906);
        SFRField<uint16_t> mode((uint16_t)burnwire_mode_type::standby, 1907);
    } // namespace burnwire
    namespace camera {
        // OP Codes 2000
        SFRField<bool> photo_taken_sd_failed(false, 2000);
        SFRField<bool> take_photo(false, 2001);
        SFRField<bool> turn_on(false, 2002);
        SFRField<bool> turn_off(false, 2003);
        SFRField<bool> powered(false, 2004);

        // Initialization
        SFRField<uint8_t> start_progress(0, 2005);
        SFRField<uint32_t> step_time(0, 2006);
        SFRField<uint32_t> init_start_time(0, 2007);
        SFRField<uint32_t> init_timeout(12000, 2008);
        SFRField<uint32_t> begin_delay(100, 2009);
        SFRField<uint32_t> resolution_set_delay(500, 2010);
        SFRField<uint32_t> resolution_get_delay(200, 2011);

        SFRField<uint16_t> init_mode((uint16_t)camera_init_mode_type::awaiting, 2012);
        SFRField<uint16_t> mode((uint16_t)sensor_mode_type::normal, 2013);

        SFRField<uint32_t> images_written(0, 2014);
        SFRField<uint32_t> fragments_written(0, 2015);

        SFRField<uint32_t> set_res(VC0706_160x120, 2016);
    } // namespace camera
    namespace rockblock {
        // OP Codes 2100
        SFRField<bool> rockblock_ready_status(false, 2100);

        SFRField<uint32_t> last_downlink(0, 2101);
        SFRField<uint32_t> downlink_period(0, 2102);

        SFRField<bool> waiting_message(false, 2103);

        SFRField<uint8_t> max_commands_count(10, 2104);

        SFRField<uint16_t> imu_max_fragments(256, 2105);

        SFRField<uint32_t> imudownlink_start_time(0, 2106);
        SFRField<uint32_t> imudownlink_remain_time(constants::time::one_minute, 2107);
        SFRField<bool> imu_first_start(true, 2108);
        SFRField<bool> imu_downlink_on(true, 2109);

        SFRField<bool> flush_status(false, 2108);
        SFRField<bool> waiting_command(false, 2109);
        SFRField<uint32_t> conseq_reads(0, 2110);
        SFRField<uint32_t> timeout(10 * constants::time::one_minute, 2111);
        SFRField<uint32_t> start_time(0, 2112);
        SFRField<uint32_t> start_time_check_signal(0, 2113);
        SFRField<uint32_t> max_check_signal_time(constants::time::one_minute, 2114);
        SFRField<bool> sleep_mode(false, 2115);

        SFRField<uint16_t> downlink_report_type((uint16_t)report_type::normal_report, 2116);
        SFRField<uint16_t> mode((uint16_t)rockblock_mode_type::send_at, 2117);

        std::deque<uint8_t> imu_report;
    } // namespace rockblock
    namespace imu {
        // OP Codes 2200
        SFRField<uint16_t> mode(sensor_mode_type::init, 2200);
        SFRField<bool> successful_init(true, 2201);

        SFRField<uint32_t> max_fragments(256, 2202);

        SFRField<bool> sample(true, 2203);
        SFRField<bool> sample_gyro(true, 2204);

        SensorReading *mag_x_value = new SensorReading(1, 0, 0);
        SensorReading *mag_y_value = new SensorReading(1, 0, 0);
        SensorReading *mag_z_value = new SensorReading(1, 0, 0);

        SensorReading *gyro_x_value = new SensorReading(1, 0, 0);
        SensorReading *gyro_y_value = new SensorReading(1, 0, 0);
        SensorReading *gyro_z_value = new SensorReading(1, 0, 0);

        SensorReading *mag_x_average = new SensorReading(fault_index_type::mag_x, 20, 0, 0);
        SensorReading *mag_y_average = new SensorReading(fault_index_type::mag_y, 20, 0, 0);
        SensorReading *mag_z_average = new SensorReading(fault_index_type::mag_z, 20, 0, 0);

        SensorReading *gyro_x_average = new SensorReading(fault_index_type::gyro_x, 20, 0, 0);
        SensorReading *gyro_y_average = new SensorReading(fault_index_type::gyro_y, 20, 0, 0);
        SensorReading *gyro_z_average = new SensorReading(fault_index_type::gyro_z, 20, 0, 0);

        SensorReading *acc_x_average = new SensorReading(fault_index_type::acc_x, 20, 0, 0);
        SensorReading *acc_y_average = new SensorReading(fault_index_type::acc_y, 20, 0, 0);

        std::deque<uint8_t> imu_dlink;

        boolean report_written = false;

    } // namespace imu
    namespace temperature {
        // OP Codes 2300
        SFRField<bool> in_sun(false, 2300);
    } // namespace temperature
    namespace current {
        // OP Codes 2400
        SFRField<bool> in_sun(false, 2400);
    } // namespace current
    namespace acs {
        // OP Codes 2500
        SFRField<uint32_t> max_no_communication(0, 2500);

        SFRField<uint32_t> on_time(5 * constants::time::one_minute, 2501);
        SFRField<bool> off(true, 2502);

        SFRField<uint16_t> mag((uint16_t)simple_acs_type::x, 2503);
    } // namespace acs
    namespace battery {
        // OP Codes 2600
        // TODO
        SFRField<uint32_t> acceptable_battery(0, 2600);
        SFRField<uint32_t> min_battery(0, 2601);
    } // namespace battery
    namespace button {
        // OP Codes 2700
        SFRField<bool> pressed(true, 2700);
    } // namespace button
    namespace EEPROM {
        // OP Codes 2800
        SFRField<uint32_t> time_of_last_write(0, 2800);
        SFRField<uint32_t> write_step_time(1000, 2801); // the amount of time between each write to EEPROM
        SFRField<uint32_t> alloted_time(7200000, 2802); // the amount of time for the EEPROM to count to (7200000 ms = 2 h)
        SFRField<uint32_t> eeprom_value(0, 2803);       // the amount of time that the EEPROM has counted, stops when the alloted time has been reached
        SFRField<bool> alloted_time_passed(false, 2804);
    } // namespace EEPROM
};    // namespace sfr

#endif
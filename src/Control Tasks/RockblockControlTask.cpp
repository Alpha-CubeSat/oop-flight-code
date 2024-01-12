#include "RockblockControlTask.hpp"
#include "EEPROMResetCommand.hpp"

RockblockControlTask::RockblockControlTask()
{
    sfr::rockblock::serial.begin(constants::rockblock::baud);
}

void RockblockControlTask::execute()
{
    rockblock_mode_type mode = static_cast<rockblock_mode_type>(sfr::rockblock::mode.get());

    if (sfr::rockblock::sleep_mode) {
        transition_to(rockblock_mode_type::standby);
    }

#ifdef VERBOSE
    Serial.print("RockBLOCK Same Mode: ");
    Serial.println(same_mode);
#endif

    if (mode != rockblock_mode_type::standby) {
        if (same_mode > constants::rockblock::max_same_mode) {
            transition_to(rockblock_mode_type::standby);
        }
        same_mode++;
    }

    switch (mode) {
    case rockblock_mode_type::standby:
        dispatch_standby();
        break;
    case rockblock_mode_type::send_at:
        // AT\r
        dispatch_send_at();
        break;
    case rockblock_mode_type::await_at:
        // OK\r
        dispatch_await_at();
        break;
    case rockblock_mode_type::send_signal_strength:
        // AT+CSQ\r
        dispatch_send_signal_strength();
        break;
    case rockblock_mode_type::await_signal_strength:
        // +CSQ: <rssi>
        dispatch_await_signal_strength();
        break;
    case rockblock_mode_type::send_flow_control:
        // AT&K0\r
        dispatch_send_flow_control();
        break;
    case rockblock_mode_type::await_flow_control:
        // OK\r
        dispatch_await_flow_control();
        break;
    case rockblock_mode_type::send_message_length:
        // AT+SBDWB=[<SBD Message Length>]\r
        dispatch_send_message_length();
        break;
    case rockblock_mode_type::await_message_length:
        // READY\r
        dispatch_await_message_length();
        break;
    case rockblock_mode_type::send_message:
        // <Binary Message>+<Checksum>\r
        dispatch_send_message();
        break;
    case rockblock_mode_type::await_message:
        // <Status>\r
        dispatch_await_message();
        break;
    case rockblock_mode_type::await_message_ok:
        // OK\r
        dispatch_await_message_ok();
        break;
    case rockblock_mode_type::send_response:
        // AT+SBDIX\r
        dispatch_send_response();
        break;
    case rockblock_mode_type::create_buffer:
        // +SBDIX: <MO status>, <MOMSN>, <MT status>, <MTMSN>, <MT length>, <MT queued>\r
        dispatch_create_buffer();
        break;
    case rockblock_mode_type::process_mo_status:
        dispatch_process_mo_status();
        break;
    case rockblock_mode_type::send_signal_strength_mo:
        // AT+CSQ\r
        dispatch_send_signal_strength_mo();
        break;
    case rockblock_mode_type::await_signal_strength_mo:
        // +CSQ: <rssi>
        dispatch_await_signal_strength_mo();
        break;
    case rockblock_mode_type::process_mt_status:
        dispatch_process_mt_status();
        break;
    case rockblock_mode_type::read_message_ok:
        // OK\r
        dispatch_read_message_ok();
        break;
    case rockblock_mode_type::read_message:
        // SENT: AT+SBDRBr
        dispatch_read_message();
        break;
    case rockblock_mode_type::process_command:
        dispatch_process_command();
        break;
    case rockblock_mode_type::queue_check:
        dispatch_queue_check();
        break;
    case rockblock_mode_type::send_flush:
        // AT+SBDWT=FLUSH_MT\r
        dispatch_send_flush();
        break;
    case rockblock_mode_type::await_flush:
        // OK\r
        dispatch_await_flush();
        break;
    case rockblock_mode_type::end_transmission:
        dispatch_end_transmission();
        break;
    }
}

void RockblockControlTask::dispatch_standby()
{
#ifdef VERBOSE
    if (sfr::rockblock::ready_status) {
        Serial.print("Rockblock Ready to Downlink\n");
    } else {
        Serial.print("Rockblock Not Ready to Downlink\n");
    }
#endif

    if (sfr::rockblock::ready_status && !sfr::rockblock::sleep_mode) {
        transition_to(rockblock_mode_type::send_at);
        Pins::setPinState(constants::rockblock::sleep_pin, HIGH);
    } else {
        Pins::setPinState(constants::rockblock::sleep_pin, LOW);
    }
}

void RockblockControlTask::dispatch_send_at()
{
    conseq_reads = 0;
#ifdef VERBOSE
    Serial.println("SENT: ATr");
#endif
    sfr::rockblock::serial.print("AT\r");
    transition_to(rockblock_mode_type::await_at);
}

void RockblockControlTask::dispatch_await_at()
{
    if (get_OK()) {
#ifdef VERBOSE
        Serial.println("SAT INFO: ok");
#endif
        transition_to(rockblock_mode_type::send_signal_strength);
    }
}

void RockblockControlTask::dispatch_send_signal_strength()
{
#ifdef VERBOSE
    Serial.println("SENT: AT+CSQr");
#endif
    sfr::rockblock::serial.print("AT+CSQ\r");
    transition_to(rockblock_mode_type::await_signal_strength);
}

void RockblockControlTask::dispatch_await_signal_strength()
{
    get_valid_signal(rockblock_mode_type::send_flow_control, rockblock_mode_type::send_signal_strength);
}

void RockblockControlTask::dispatch_send_flow_control()
{
#ifdef VERBOSE
    Serial.println("SENT: AT&K0r");
#endif
    sfr::rockblock::serial.print("AT&K0\r");
    transition_to(rockblock_mode_type::await_flow_control);
}

void RockblockControlTask::dispatch_await_flow_control()
{
    if (get_OK()) {
#ifdef VERBOSE
        Serial.println("SAT INFO: ok");
#endif
        transition_to(rockblock_mode_type::send_message_length);
    }
}

void RockblockControlTask::dispatch_send_message_length()
{
    std::stringstream ss;
    ss << sfr::rockblock::downlink_report.size();
    std::string s = ss.str();
    std::string message_length = "AT+SBDWB=" + s + "\r";
#ifdef VERBOSE
    Serial.println(("SENT: AT+SBDWB=" + s + "r").c_str());
#endif
    sfr::rockblock::serial.print(message_length.c_str());
    transition_to(rockblock_mode_type::await_message_length);
}

void RockblockControlTask::dispatch_await_message_length()
{
    if (sfr::rockblock::serial.read() == 'R' &&
        sfr::rockblock::serial.read() == 'E' &&
        sfr::rockblock::serial.read() == 'A' &&
        sfr::rockblock::serial.read() == 'D' &&
        sfr::rockblock::serial.read() == 'Y' &&
        sfr::rockblock::serial.read() == '\r') {
#ifdef VERBOSE
        Serial.println("SAT INFO: ready");
#endif
        transition_to(rockblock_mode_type::send_message);
    }
}

void RockblockControlTask::dispatch_send_message()
{
    uint16_t checksum = 0;
#ifdef VERBOSE
    switch (static_cast<report_type>(sfr::rockblock::downlink_report_type.get())) {
    case report_type::camera_report: {
        Serial.println("==========================================================================");
        Serial.println("Optical Sensor Data Report Downlinking");
        Serial.println("==========================================================================");
        Serial.println("Item Name            -> Raw Hex         -> Deserialized & Decimal         ");
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("Start Flag->");
        print_hex(sfr::rockblock::downlink_report[0]);
        Serial.print("->");
        Serial.println(sfr::rockblock::downlink_report[0]);
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("Serial Number->");
        print_hex(sfr::rockblock::downlink_report[1]);
        Serial.print("->");
        Serial.println(sfr::rockblock::downlink_report[1]);
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("Fragment Number->");
        print_hex(sfr::rockblock::downlink_report[2]);
        print_hex(sfr::rockblock::downlink_report[3]);
        print_hex(sfr::rockblock::downlink_report[4]);
        print_hex(sfr::rockblock::downlink_report[5]);
        Serial.print("->");
        uint32_t fragment_number = 0;
        fragment_number = (sfr::rockblock::downlink_report[2] << 24) + (sfr::rockblock::downlink_report[3] << 16) + (sfr::rockblock::downlink_report[4] << 8) + sfr::rockblock::downlink_report[5];
        Serial.println(fragment_number);
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("Image: ");
        for (uint8_t i = 6; i < sfr::rockblock::downlink_report.size(); i++) {
            print_hex(sfr::rockblock::downlink_report[i]);
        }
        Serial.println();
        Serial.println("==========================================================================");
        break;
    }
    case report_type::imu_report:
        Serial.println("==========================================================================");
        Serial.println("IMU Report Downlinking");
        Serial.println("==========================================================================");
        Serial.println("Item Name            -> Raw Hex         -> Deserialized & Decimal         ");
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("Start Flag->");
        print_hex(sfr::rockblock::downlink_report[0]);
        Serial.print("->");
        Serial.println(sfr::rockblock::downlink_report[0]);
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("Fragment Number: ");
        print_hex(sfr::rockblock::downlink_report[1]);
        Serial.print("->");
        Serial.println(sfr::rockblock::downlink_report[1]);
        Serial.println("--------------------------------------------------------------------------");

        for (uint8_t i = 2; i < sfr::rockblock::downlink_report.size() - 2; i = i + 3) {
            Serial.print("Gyro X: ");
            print_SensorReading(i, sfr::imu::gyro_x_value);

            Serial.print("Gyro Y: ");
            print_SensorReading(i + 1, sfr::imu::gyro_y_value);

            Serial.print("Gyro Z: ");
            print_SensorReading(i + 2, sfr::imu::gyro_z_value);
            Serial.println("--------------------------------------------------------------------------");
        }

        Serial.print("End Flag 1: ");
        print_hex(sfr::rockblock::downlink_report[sfr::rockblock::downlink_report.size() - 2]);
        Serial.print("->");
        Serial.println(sfr::rockblock::downlink_report[sfr::rockblock::downlink_report.size() - 2]);
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("End Flag 2: ");
        print_hex(sfr::rockblock::downlink_report[sfr::rockblock::downlink_report.size() - 1]);
        Serial.print("->");
        Serial.println(sfr::rockblock::downlink_report[sfr::rockblock::downlink_report.size() - 1]);
        Serial.println("==========================================================================");
        break;
    case report_type::normal_report:
        Serial.println("==========================================================================");
        Serial.println("Normal Report Downlinking");
        Serial.println("==========================================================================");
        Serial.println("Item Name            -> Raw Hex         -> Deserialized & Decimal         ");
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("Start Flag->");
        print_hex(sfr::rockblock::downlink_report[0]);
        Serial.print("->");
        Serial.println(sfr::rockblock::downlink_report[0]);
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("sfr::mission::boot_time_mins->");
        print_SFRField(1, 0x1802);

        Serial.print("sfr::burnwire::burn_time->");
        print_SFRField(2, 0x1905);

        Serial.print("sfr::burnwire::armed_time->");
        print_SFRField(3, 0x1906);

        Serial.print("sfr::rockblock::lp_downlink_period->");
        print_SFRField(4, 0x2109);

        Serial.print("sfr::rockblock::transmit_downlink_period->");
        print_SFRField(5, 0x2110);

        Serial.print("sfr::acs::mode->");
        print_SFRField(6, 0x2501);

        Serial.print("sfr::acs::Id_index->");
        print_SFRField(7, 0x2505);

        Serial.print("sfr::acs::Kd_index->");
        print_SFRField(8, 0x2506);

        Serial.print("sfr::acs::Kp_index->");
        print_SFRField(9, 0x2507);

        Serial.print("sfr::acs::c_index->");
        print_SFRField(10, 0x2508);

        Serial.print("sfr::eeprom::boot_counter->");
        print_SFRField(11, 0x2805);

        Serial.print("sfr::eeprom::dynamic_data_addr->");
        print_SFRField(12, 0x2806);

        Serial.print("sfr::eeprom::sfr_data_addr->");
        print_SFRField(13, 0x2807);

        Serial.print("sfr::eeprom::time_alive->");
        print_SFRField(14, 0x2808);

        Serial.print("sfr::eeprom::dynamic_data_age->");
        print_SFRField(15, 0x2809);

        Serial.print("sfr::eeprom::sfr_data_age->");
        print_SFRField(16, 0x2810);

        Serial.print("sfr::acs::on_time->");
        print_SFRField(17, 0x2504);

        Serial.print("sfr::rockblock::on_time->");
        print_SFRField(18, 0x2111);

        Serial.print("sfr::photoresistor::covered->");
        Serial.println((bool)(sfr::rockblock::downlink_report[19] & (1 << 0)));
        Serial.print("sfr::mission::possible_uncovered->");
        Serial.println((bool)(sfr::rockblock::downlink_report[19] & (1 << 1)));
        Serial.print("sfr::optical_sensor::powered->");
        Serial.println((bool)(sfr::rockblock::downlink_report[19] & (1 << 2)));
        Serial.print("sfr::mission::deployed->");
        Serial.println((bool)(sfr::rockblock::downlink_report[19] & (1 << 3)));
        Serial.print("sfr::rockblock::waiting_command->");
        Serial.println((bool)(sfr::rockblock::downlink_report[19] & (1 << 4)));
        Serial.print("sfr::temperature::in_sun->");
        Serial.println((bool)(sfr::rockblock::downlink_report[19] & (1 << 5)));
        Serial.print("sfr::current::in_sun->");
        Serial.println((bool)(sfr::rockblock::downlink_report[19] & (1 << 6)));
        Serial.print("sfr::button::pressed->");
        Serial.println((bool)(sfr::rockblock::downlink_report[19] & (1 << 7)));
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("sfr::photoresistor::light_val_average_standby->");
        print_SensorReading(20, sfr::photoresistor::light_val_average_standby);

        Serial.print("sfr::imu::mag_x_average->");
        print_SensorReading(21, sfr::imu::mag_x_average);

        Serial.print("sfr::imu::mag_y_average->");
        print_SensorReading(22, sfr::imu::mag_y_average);

        Serial.print("sfr::imu::mag_z_average->");
        print_SensorReading(23, sfr::imu::mag_z_average);

        Serial.print("sfr::imu::gyro_x_average->");
        print_SensorReading(24, sfr::imu::gyro_x_average);

        Serial.print("sfr::imu::gyro_y_average->");
        print_SensorReading(25, sfr::imu::gyro_y_average);

        Serial.print("sfr::imu::gyro_z_average->");
        print_SensorReading(26, sfr::imu::gyro_z_average);

        Serial.print("sfr::temperature::temp_c_value->");
        print_SensorReading(27, sfr::temperature::temp_c_value);

        Serial.print("sfr::temperature::temp_c_average->");
        print_SensorReading(28, sfr::temperature::temp_c_average);

        Serial.print("sfr::current::solar_current_average->");
        print_SensorReading(29, sfr::current::solar_current_average);

        Serial.print("sfr::battery::voltage_value->");
        print_SensorReading(30, sfr::battery::voltage_value);

        Serial.print("sfr::battery::voltage_average->");
        print_SensorReading(31, sfr::battery::voltage_average);

        Serial.println("fault_groups::imu_faults::mag_x_average->");
        print_Fault(32, false);
        Serial.println("fault_groups::imu_faults::mag_x_value->");
        print_Fault(32, true);

        Serial.println("fault_groups::imu_faults::mag_y_average->");
        print_Fault(33, false);
        Serial.println("fault_groups::imu_faults::mag_y_value->");
        print_Fault(33, true);

        Serial.println("fault_groups::imu_faults::mag_z_average->");
        print_Fault(34, false);
        Serial.println("fault_groups::imu_faults::mag_z_value->");
        print_Fault(34, true);

        Serial.println("fault_groups::imu_faults::gyro_x_average->");
        print_Fault(35, false);
        Serial.println("fault_groups::imu_faults::gyro_x_value->");
        print_Fault(35, true);

        Serial.println("fault_groups::imu_faults::gyro_y_average->");
        print_Fault(36, false);
        Serial.println("fault_groups::imu_faults::gyro_y_value->");
        print_Fault(36, true);

        Serial.println("fault_groups::imu_faults::gyro_z_average->");
        print_Fault(37, false);
        Serial.println("fault_groups::imu_faults::gyro_z_value->");
        print_Fault(37, true);

        Serial.println("fault_groups::power_faults::temp_c_average->");
        print_Fault(38, false);
        Serial.println("fault_groups::power_faults::temp_c_value->");
        print_Fault(38, true);

        Serial.println("fault_groups::power_faults::voltage_average->");
        print_Fault(39, false);
        Serial.println("fault_groups::power_faults::voltage_value->");
        print_Fault(39, true);

        Serial.println("fault_groups::hardware_faults::button->");
        print_Fault(40, false);
        Serial.println("fault_groups::hardware_faults::light_val->");
        print_Fault(40, true);

        Serial.println("fault_groups::power_faults::solar_current_average->");
        print_Fault(41, true);

        Serial.print("sfr::eeprom::boot_restarted->");
        Serial.println((bool)(sfr::rockblock::downlink_report[41] & (1 << 0)));
        Serial.print("sfr::eeprom::error_mode->");
        Serial.println((bool)(sfr::rockblock::downlink_report[41] & (1 << 1)));
        Serial.print("sfr::eeprom::light_switch->");
        Serial.println((bool)(sfr::rockblock::downlink_report[41] & (1 << 2)));
        Serial.print("sfr::eeprom::sfr_save_completed->");
        Serial.println((bool)(sfr::rockblock::downlink_report[41] & (1 << 3)));
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("Mission Mode History->");
        print_hex(sfr::rockblock::downlink_report[42]);
        print_hex(sfr::rockblock::downlink_report[43]);
        print_hex(sfr::rockblock::downlink_report[44]);
        print_hex(sfr::rockblock::downlink_report[45]);
        print_hex(sfr::rockblock::downlink_report[46]);
        print_hex(sfr::rockblock::downlink_report[47]);
        print_hex(sfr::rockblock::downlink_report[48]);
        print_hex(sfr::rockblock::downlink_report[49]);
        print_hex(sfr::rockblock::downlink_report[50]);
        print_hex(sfr::rockblock::downlink_report[51]);
        Serial.println();
        Serial.println("--------------------------------------------------------------------------");

        Serial.print("Processed Opcodes->");
        print_hex(sfr::rockblock::downlink_report[52]);
        print_hex(sfr::rockblock::downlink_report[53]);
        print_hex(sfr::rockblock::downlink_report[54]);
        print_hex(sfr::rockblock::downlink_report[55]);
        print_hex(sfr::rockblock::downlink_report[56]);
        print_hex(sfr::rockblock::downlink_report[57]);
        print_hex(sfr::rockblock::downlink_report[58]);
        print_hex(sfr::rockblock::downlink_report[59]);
        print_hex(sfr::rockblock::downlink_report[60]);
        print_hex(sfr::rockblock::downlink_report[61]);
        print_hex(sfr::rockblock::downlink_report[62]);
        print_hex(sfr::rockblock::downlink_report[63]);
        print_hex(sfr::rockblock::downlink_report[64]);
        print_hex(sfr::rockblock::downlink_report[65]);
        print_hex(sfr::rockblock::downlink_report[66]);
        print_hex(sfr::rockblock::downlink_report[67]);
        print_hex(sfr::rockblock::downlink_report[68]);
        print_hex(sfr::rockblock::downlink_report[69]);
        Serial.println();
        Serial.println("==========================================================================");
        break;
    }
    Serial.print("SENT: ");
#endif
    for (auto &data : sfr::rockblock::downlink_report) {
        sfr::rockblock::serial.write(data);
#ifdef VERBOSE
        print_hex(data);
#endif
        checksum += (uint16_t)data;
    }

#ifdef VERBOSE
    Serial.println();
    Serial.print("SENT: ");
    Serial.print(checksum >> 8);
    Serial.print(checksum & 0xFF);
    Serial.print('r');
    Serial.println();
#endif

    sfr::rockblock::serial.write(checksum >> 8);
    sfr::rockblock::serial.write(checksum & 0xFF);
    sfr::rockblock::serial.write('\r');
    transition_to(rockblock_mode_type::await_message);
}

void RockblockControlTask::dispatch_await_message()
{
    char c = sfr::rockblock::serial.read();
    if (c == '0' || c == '1' || c == '2' || c == '3') {
        if (c == '0') {
#ifdef VERBOSE
            Serial.println("SAT INFO: report accepted");
#endif
            transition_to(rockblock_mode_type::await_message_ok);
        } else {
            transition_to(rockblock_mode_type::send_message);
        }
    }
}

void RockblockControlTask::dispatch_await_message_ok()
{
    if (get_OK()) {
        Serial.println("SAT INFO: OK");
        transition_to(rockblock_mode_type::send_response);
    }
}

void RockblockControlTask::dispatch_send_response()
{
#ifdef VERBOSE
    Serial.println("SENT: AT+SBDIXr");
#endif
    sfr::rockblock::serial.print("AT+SBDIX\r");
    transition_to(rockblock_mode_type::create_buffer);
}

void RockblockControlTask::dispatch_create_buffer()
{
    if (sfr::rockblock::serial.read() == '+' &&
        sfr::rockblock::serial.read() == 'S' &&
        sfr::rockblock::serial.read() == 'B' &&
        sfr::rockblock::serial.read() == 'D' &&
        sfr::rockblock::serial.read() == 'I' &&
        sfr::rockblock::serial.read() == 'X' &&
        sfr::rockblock::serial.read() == ':') {
        // clear buffer to nulls
        memset(sfr::rockblock::buffer, '\0', constants::rockblock::buffer_size);
        // clear commas to -1
        memset(sfr::rockblock::commas, -1, constants::rockblock::num_commas);
        uint8_t buffer_iter = 0;
        int comma_iter = 0;
        for (size_t i = 0; i < constants::rockblock::buffer_size; i++) {
            char c = sfr::rockblock::serial.read();
            if (c == '\r') {
                break;
            }
            if (c != ' ') {
                sfr::rockblock::buffer[buffer_iter] = c;
                Serial.print(sfr::rockblock::buffer[buffer_iter]);
                if (c == ',') {
                    sfr::rockblock::commas[comma_iter] = buffer_iter;
                    comma_iter++;
                }
                buffer_iter++;
            }
        }
        Serial.println();
        if (comma_iter != 5) {
            transition_to(rockblock_mode_type::send_response);
        } else {
            transition_to(rockblock_mode_type::process_mo_status);
        }
    }
}

void RockblockControlTask::dispatch_process_mo_status()
{
    if (sfr::rockblock::commas[0] > 1) {
        Serial.println("SAT INFO: there is another character");
        if (sfr::mission::current_mode->get_id() == sfr::mission::aliveSignal->get_id()) {
#ifdef VERBOSE
            Serial.println("ROCKBLOCK HARD FAULT");
#endif
            sfr::aliveSignal::num_hard_faults++;
        }
        transition_to(rockblock_mode_type::send_signal_strength_mo);
    } else if (sfr::rockblock::buffer[0] != '0' && sfr::rockblock::buffer[0] != '1' && sfr::rockblock::buffer[0] != '2') {
        Serial.println("SAT INFO: mo status is greater than 2");
        transition_to(rockblock_mode_type::send_response);
    } else {
        transition_to(rockblock_mode_type::process_mt_status);
    }
}

void RockblockControlTask::dispatch_send_signal_strength_mo()
{
    Serial.println("SENT: AT+CSQr");
    sfr::rockblock::serial.print("AT+CSQ\r");
    transition_to(rockblock_mode_type::await_signal_strength_mo);
}

void RockblockControlTask::dispatch_await_signal_strength_mo()
{
    get_valid_signal(rockblock_mode_type::send_response, rockblock_mode_type::send_signal_strength_mo);
}

void RockblockControlTask::dispatch_process_mt_status()
{
    switch (sfr::rockblock::buffer[sfr::rockblock::commas[1] + 1]) {
    case '2':
        Serial.println("SAT INFO: error during check");
        transition_to(rockblock_mode_type::send_response);
        break;
    case '1':
        Serial.println("SAT INFO: message retrieved");
        if (sfr::rockblock::downlink_report_type == (uint16_t)report_type::camera_report) {
            sfr::rockblock::camera_report.clear();
        }
        if (sfr::rockblock::downlink_report_type == (uint16_t)report_type::imu_report) {
            sfr::rockblock::imu_report.clear();
        }
        transition_to(rockblock_mode_type::read_message_ok);
        break;
    case '0':
        Serial.println("SAT INFO: there were no messages to retrieve");
        transition_to(rockblock_mode_type::end_transmission);
        break;
    }
}

void RockblockControlTask::dispatch_read_message_ok()
{
    if (get_OK()) {
        sfr::rockblock::flush_status = true;
        Serial.println("SAT INFO: OK");
        transition_to(rockblock_mode_type::read_message);
    }
}

void RockblockControlTask::dispatch_read_message()
{
    Serial.println("SENT: AT+SBDRBr");
    sfr::rockblock::serial.print("AT+SBDRB\r");
    transition_to(rockblock_mode_type::process_command);
}

void RockblockControlTask::dispatch_process_command()
{
    // read until start of command flags read
    if (sfr::rockblock::serial.read() == constants::rockblock::start_of_command_upload_flag1 &&
        sfr::rockblock::serial.read() == constants::rockblock::start_of_command_upload_flag2) {
        /*
            Parses up to `max_commands_count` number of commands
            Exits early if end-of-command-upload flags read
        */
        for (int i = 0; i < sfr::rockblock::max_commands_count; i++) {
            look_ahead1 = sfr::rockblock::serial.read();
            look_ahead2 = sfr::rockblock::serial.read();
            if (look_ahead1 == constants::rockblock::end_of_command_upload_flag1 &&
                look_ahead2 == constants::rockblock::end_of_command_upload_flag2) {
                transition_to(rockblock_mode_type::queue_check);
                return; // Exit command read loop
            }
            Serial.println("SAT CMD");
            // Instantiate a new unprocessed raw command
            RawRockblockCommand new_raw_command;
            new_raw_command.opcode[0] = look_ahead1;
            new_raw_command.opcode[1] = look_ahead2;

            if (look_ahead1 < 0x10)
                Serial.print(0, HEX);
            Serial.print(look_ahead1, HEX);

            if (look_ahead2 < 0x10)
                Serial.print(0, HEX);
            Serial.print(look_ahead2, HEX);

            for (size_t a1 = 0; a1 < constants::rockblock::arg1_len; ++a1) {
                new_raw_command.arg_1[a1] = sfr::rockblock::serial.read();
                if (new_raw_command.arg_1[a1] < 0x10)
                    Serial.print(0, HEX);
                Serial.print(new_raw_command.arg_1[a1], HEX);
            }
            for (size_t a2 = 0; a2 < constants::rockblock::arg2_len; ++a2) {
                new_raw_command.arg_2[a2] = sfr::rockblock::serial.read();
                if (new_raw_command.arg_2[a2] < 0x10)
                    Serial.print(0, HEX);
                Serial.print(new_raw_command.arg_2[a2], HEX);
            }

            Serial.println();

            // Parse New Command From Input OP Codes
            RockblockCommand *processed = commandFactory(new_raw_command);
            if (processed->isValid()) {
                // Command is Valid - Will be added to list to be Executed During CommandMonitor Execute
                sfr::rockblock::processed_commands.push_back(processed);
                sfr::rockblock::waiting_command = true;
            } else {
                Serial.println("SAT INFO: invalid command");
            }
        }

        conseq_reads++;
        transition_to(rockblock_mode_type::queue_check);
    }
}

void RockblockControlTask::dispatch_queue_check()
{
    size_t idx = sfr::rockblock::commas[4] + 1;
    char *ptr = sfr::rockblock::buffer + idx;
    int queued = strtol(ptr, nullptr, 10);
    Serial.print("SAT INFO: ");
    Serial.print(queued);
    Serial.println(" waiting");
    // check if in cycle before limit
    bool cycle_boundary = conseq_reads >= (constants::rockblock::max_conseq_read - 1);
    // check if enough messages are waiting
    bool queue_limit = queued > 1;
    if (cycle_boundary && queue_limit && !sfr::rockblock::flush_status) {
        transition_to(rockblock_mode_type::send_flush);
    } else if (queued > 0) {
        transition_to(rockblock_mode_type::send_message_length);
    } else {
        transition_to(rockblock_mode_type::end_transmission);
    }
}

void RockblockControlTask::dispatch_send_flush()
{
    Serial.println("SENT: AT+SBDWT=FLUSH_MTr");
    sfr::rockblock::serial.print("AT+SBDWT=FLUSH_MT\r");
    transition_to(rockblock_mode_type::await_flush);
}

void RockblockControlTask::dispatch_await_flush()
{
    if (get_OK()) {
        sfr::rockblock::flush_status = true;
        Serial.println("SAT INFO: OK");
        transition_to(rockblock_mode_type::send_response);
    }
}

void RockblockControlTask::dispatch_end_transmission()
{
    sfr::rockblock::last_downlink = millis();
    sfr::aliveSignal::downlinked = true;
    if (sfr::rockblock::downlink_period > constants::rockblock::min_sleep_period) {
        Pins::setPinState(constants::rockblock::sleep_pin, LOW);
    }
    if (sfr::rockblock::downlink_report_type == (uint16_t)report_type::camera_report) {
        sfr::rockblock::camera_report.clear();
    }
    if (sfr::rockblock::downlink_report_type == (uint16_t)report_type::imu_report) {
        sfr::rockblock::imu_report.clear();
    }
    transition_to(rockblock_mode_type::standby);
}

void RockblockControlTask::transition_to(rockblock_mode_type new_mode)
{
    sfr::rockblock::mode = (uint16_t)new_mode;
    same_mode = 0;
}

RockblockCommand *RockblockControlTask::commandFactory(RawRockblockCommand raw)
{
    // Create Specific Child Class of Rockblock command depending on the OP Code
    uint16_t op_code = raw.get_f_opcode();
    if (op_code == constants::rockblock::opcodes::deploy_opcode) {
#ifdef VERBOSE
        Serial.println("Deploy Command");
#endif
        return new DeployCommand(raw);
    }

    else if (op_code == constants::rockblock::opcodes::arm_opcode) {
#ifdef VERBOSE
        Serial.println("Arm Command");
#endif
        return new ArmCommand(raw);
    }

    else if (op_code == constants::rockblock::opcodes::fire_opcode) {
#ifdef VERBOSE
        Serial.println("Fire Command");
#endif
        return new FireCommand(raw);
    }

    else if (op_code == constants::rockblock::opcodes::eeprom_reset_opcode) {
#ifdef VERBOSE
        Serial.println("EEPROM Reset Command");
#endif
        return new EEPROMResetCommand(raw);
    }

    else if (op_code == constants::rockblock::opcodes::camera_fragment_request_opcode) {
#ifdef VERBOSE
        Serial.println("Optical Sensor Fragment Request");
#endif
        return new CameraFragmentCommand(raw);
    }

    else if (op_code == constants::rockblock::opcodes::mission_mode_override_opcode) {
#ifdef VERBOSE
        Serial.println("Mission Mode Override Command");
#endif
    }

    else if (op_code >= constants::rockblock::opcodes::sfr_field_opcode_min && op_code <= constants::rockblock::opcodes::sfr_field_opcode_max) {
#ifdef VERBOSE
        Serial.println("SFR Override Command");
#endif
        return new SFROverrideCommand(raw);
    }

    else if (op_code >= constants::rockblock::opcodes::fault_opcode_min && op_code <= constants::rockblock::opcodes::fault_opcode_max) {
#ifdef VERBOSE
        Serial.println("Fault Override Command");
#endif
        return new FaultOverrideCommand(raw);
    }

    else {
#ifdef VERBOSE
        Serial.print("Unknown Command with opcode: ");
        Serial.println(op_code, HEX);
#endif
        return new UnknownCommand(raw);
    }
}

bool RockblockControlTask::get_OK()
{
    if (sfr::rockblock::serial.read() == 'O' &&
        sfr::rockblock::serial.read() == 'K' &&
        sfr::rockblock::serial.read() == '\r') {
        return true;
    }
    return false;
}

void RockblockControlTask::get_valid_signal(rockblock_mode_type good_signal, rockblock_mode_type bad_signal)
{
    if (sfr::rockblock::serial.read() == '+' &&
        sfr::rockblock::serial.read() == 'C' &&
        sfr::rockblock::serial.read() == 'S' &&
        sfr::rockblock::serial.read() == 'Q' &&
        sfr::rockblock::serial.read() == ':') {
        char signal = sfr::rockblock::serial.read();
#ifdef VERBOSE
        Serial.print("SAT INFO: signal level ");
        Serial.println(signal);
#endif
        if (signal == '3' || signal == '4' || signal == '5') {
            transition_to(good_signal);
        } else {
            transition_to(bad_signal);
        }
    }
}

void RockblockControlTask::print_hex(uint8_t hex_num)
{
    if (hex_num < 16) {
        Serial.print(0);
    }
    Serial.print(hex_num, HEX);
}

float RockblockControlTask::deserialize(float value, float min, float max)
{
    if ((max - min) == 0) {
        return 0;
    }
    return map(value, 0, 255, min, max);
}

void RockblockControlTask::print_SFRField(int i, int opcode)
{
    SFRInterface *valueObj = SFRInterface::opcode_lookup[opcode];
    print_hex(sfr::rockblock::downlink_report[i]);
    Serial.print("->");
    Serial.println(deserialize(sfr::rockblock::downlink_report[i], valueObj->getMin(), valueObj->getMax()));
    Serial.println("--------------------------------------------------------------------------");
}

void RockblockControlTask::print_SensorReading(int i, SensorReading *valueObj)
{
    print_hex(sfr::rockblock::downlink_report[i]);
    Serial.print("->");
    Serial.println(deserialize(sfr::rockblock::downlink_report[i], valueObj->get_min(), valueObj->get_max()));
    Serial.println("--------------------------------------------------------------------------");
}

void RockblockControlTask::print_Fault(int i, bool shift)
{
    Serial.print("Signaled: ");
    Serial.println((bool)(sfr::rockblock::downlink_report[i] & (1 << (0 + shift * 4))));

    Serial.print("Suppressed: ");
    Serial.println((bool)(sfr::rockblock::downlink_report[i] & (1 << (1 + shift * 4))));

    Serial.print("Forced: ");
    Serial.println((bool)(sfr::rockblock::downlink_report[i] & (1 << (2 + shift * 4))));

    Serial.print("Base: ");
    Serial.println((bool)(sfr::rockblock::downlink_report[i] & (1 << (3 + shift * 4))));
    Serial.println("--------------------------------------------------------------------------");
}

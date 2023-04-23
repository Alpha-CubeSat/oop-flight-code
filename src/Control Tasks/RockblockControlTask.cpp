#include "RockblockControlTask.hpp"
#include "BurnwireCommands.hpp"

RockblockControlTask::RockblockControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    sfr::rockblock::serial.begin(constants::rockblock::baud);
}

void RockblockControlTask::execute()
{
    rockblock_mode_type mode = static_cast<rockblock_mode_type>(sfr::rockblock::mode.get());
    switch (mode) {
    case rockblock_mode_type::standby:
        dispatch_standby();
        break;
    case rockblock_mode_type::send_at:
        dispatch_send_at();
        break;
    case rockblock_mode_type::await_at:
        dispatch_await_at();
        break;
    case rockblock_mode_type::send_signal_strength:
        dispatch_send_signal_strength();
        break;
    case rockblock_mode_type::await_signal_strength:
        dispatch_await_signal_strength();
        break;
    case rockblock_mode_type::send_flow_control:
        dispatch_send_flow_control();
        break;
    case rockblock_mode_type::await_flow_control:
        dispatch_await_flow_control();
        break;
    case rockblock_mode_type::send_message_length:
        dispatch_send_message_length();
        break;
    case rockblock_mode_type::await_message_length:
        dispatch_await_message_length();
        break;
    case rockblock_mode_type::send_message:
        dispatch_send_message();
        break;
    case rockblock_mode_type::await_message:
        dispatch_await_message();
        break;
    case rockblock_mode_type::send_response:
        dispatch_send_response();
        break;
    case rockblock_mode_type::create_buffer:
        dispatch_create_buffer();
        break;
    case rockblock_mode_type::process_mo_status:
        dispatch_process_mo_status();
        break;
    case rockblock_mode_type::send_signal_strength_mo:
        dispatch_send_signal_strength_mo();
        break;
    case rockblock_mode_type::await_signal_strength_mo:
        dispatch_await_signal_strength_mo();
        break;
    case rockblock_mode_type::process_mt_status:
        dispatch_process_mt_status();
        break;
    case rockblock_mode_type::read_message:
        dispatch_read_message();
        break;
    case rockblock_mode_type::process_command:
        dispatch_process_command();
        break;
    case rockblock_mode_type::queue_check:
        dispatch_queue_check();
        break;
    case rockblock_mode_type::send_flush:
        dispatch_send_flush();
        break;
    case rockblock_mode_type::await_flush:
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

#ifdef VERBOSE_RB
    if (sfr::rockblock::ready_status) {
        Serial.print("Rockblock Ready to Downlink\n");
    } else {
        Serial.print("Rockblock Not Ready to Downlink\n");
    }
#endif

    if ((sfr::rockblock::ready_status || sfr::rockblock::waiting_message) && !sfr::rockblock::sleep_mode) {
        transition_to(rockblock_mode_type::send_at);
        Pins::setPinState(constants::rockblock::sleep_pin, HIGH);
    } else {
        Pins::setPinState(constants::rockblock::sleep_pin, LOW);
    }
}

void RockblockControlTask::dispatch_send_at()
{
    sfr::rockblock::conseq_reads = 0;
#ifdef VERBOSE_RB
    Serial.println("SENT: ATr");
#endif
    sfr::rockblock::serial.print("AT\r");
    transition_to(rockblock_mode_type::await_at);
}

void RockblockControlTask::dispatch_await_at()
{
    if (sfr::rockblock::serial.read() == 'K') {
#ifdef VERBOSE_RB
        Serial.println("SAT INFO: ok");
#endif
        transition_to(rockblock_mode_type::send_signal_strength);
        sfr::rockblock::start_time_check_signal = millis();
    }
}

void RockblockControlTask::dispatch_send_signal_strength()
{
#ifdef VERBOSE_RB
    Serial.println("SENT: AT+CSQr");
#endif
    sfr::rockblock::serial.print("AT+CSQ\r");
    transition_to(rockblock_mode_type::await_signal_strength);
}

void RockblockControlTask::dispatch_await_signal_strength()
{
    if (sfr::rockblock::serial.read() == ':') {
        char signal = sfr::rockblock::serial.read();
#ifdef VERBOSE_RB
        Serial.print("SAT INFO: signal level ");
        Serial.println(signal);
#endif
        if (signal == '3' || signal == '4' || signal == '5') {
            transition_to(rockblock_mode_type::send_flow_control);
        } else {
            transition_to(rockblock_mode_type::send_signal_strength);
        }
    }
}

void RockblockControlTask::dispatch_send_flow_control()
{
#ifdef VERBOSE_RB
    Serial.println("SENT: AT&K0r");
#endif
    sfr::rockblock::serial.print("AT&K0\r");
    transition_to(rockblock_mode_type::await_flow_control);
}

void RockblockControlTask::dispatch_await_flow_control()
{
    if (sfr::rockblock::serial.read() == 'K') {
#ifdef VERBOSE_RB
        Serial.println("SAT INFO: ok");
#endif
        transition_to(rockblock_mode_type::send_message_length);
    }
}

void RockblockControlTask::dispatch_send_message_length()
{
    std::stringstream ss;
    ss << sfr::rockblock::downlink_report.size();
    // Lock the max amount of reports to be the current amount to ensure size does not change before downlink
    sfr::rockblock::normal_report_command_max = sfr::rockblock::normal_report_command_curr;
    std::string s = ss.str();
    std::string message_length = "AT+SBDWB=" + s + "\r";
#ifdef VERBOSE_RB
    Serial.println(("SENT: AT+SBDWB=" + s + "r").c_str());
#endif
    sfr::rockblock::serial.print(message_length.c_str());
    transition_to(rockblock_mode_type::await_message_length);
}

void RockblockControlTask::dispatch_await_message_length()
{
    if (sfr::rockblock::serial.read() == 'Y') {
#ifdef VERBOSE_RB
        Serial.println("SAT INFO: ready");
#endif
        transition_to(rockblock_mode_type::send_message);
    }
}

void RockblockControlTask::dispatch_send_message()
{
    uint16_t checksum = 0;
#ifdef VERBOSE_RB
    switch (static_cast<report_type>(sfr::rockblock::downlink_report_type.get())) {
    case report_type::camera_report:
        Serial.print("Camera Report Downlinking\n");
        break;
    case report_type::imu_report:
        Serial.print("IMU Report Downlinking\n");
        break;
    case report_type::normal_report:
        Serial.print("Normal Report Downlinking\n");
        break;
    }

    Serial.print("SENT: ");
#endif
    for (auto &data : sfr::rockblock::downlink_report) {
        if (data < 16) {
#ifdef VERBOSE_RB
            Serial.print(0);
#endif
        }
#ifdef VERBOSE_RB
        Serial.print(data, HEX);
#endif
        sfr::rockblock::serial.write(data);
        checksum += (uint16_t)data;
    }

#ifdef VERBOSE_RB
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
    std::deque<uint8_t> empty_commands_received;
    std::swap(sfr::rockblock::commands_received, empty_commands_received);
    transition_to(rockblock_mode_type::await_message);
}

void RockblockControlTask::dispatch_await_message()
{
    char c = sfr::rockblock::serial.read();
    if (c == '0' || c == '1' || c == '2' || c == '3') {
        if (c == '0') {
#ifdef VERBOSE_RB
            Serial.println("SAT INFO: report accepted");
#endif
            sfr::rockblock::normal_report_command_max = constants::rockblock::normal_report_command_default_max;
            transition_to(rockblock_mode_type::send_response);
        } else {
            transition_to(rockblock_mode_type::send_message);
        }
    }
}

void RockblockControlTask::dispatch_send_response()
{
#ifdef VERBOSE_RB
    Serial.println("SENT: AT+SBDIXr");
#endif
    sfr::rockblock::serial.print("AT+SBDIX\r");
    transition_to(rockblock_mode_type::create_buffer);
}

void RockblockControlTask::dispatch_create_buffer()
{
    if (sfr::rockblock::serial.read() == ':') {
        // clear buffer to nulls
        memset(sfr::rockblock::buffer, '\0', constants::rockblock::buffer_size);
        // clear commas to -1
        memset(sfr::rockblock::commas, -1, constants::rockblock::num_commas);
        int buffer_iter = 0;
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
    if (sfr::rockblock::serial.read() == ':') {
        char signal = sfr::rockblock::serial.read();
        Serial.print("SAT INFO: signal level ");
        Serial.println(signal);
        if (signal == '3' || signal == '4' || signal == '5') {
            transition_to(rockblock_mode_type::send_response);
        } else {
            transition_to(rockblock_mode_type::send_signal_strength_mo);
        }
    }
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
            Serial.print("IMU report sent\n");
            sfr::rockblock::imu_report.clear();
        }
        transition_to(rockblock_mode_type::read_message);
        break;
    case '0':
        Serial.println("SAT INFO: there were no messages to retrieve");
        transition_to(rockblock_mode_type::end_transmission);
        break;
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
    if (sfr::rockblock::serial.read() == 'B') {
        sfr::rockblock::serial.read();
        sfr::rockblock::serial.read();
        sfr::rockblock::serial.read();
        sfr::rockblock::serial.read();
        sfr::rockblock::serial.read();
        sfr::rockblock::serial.read();

        /*
            Parses up to `max_commands_count` number of commands
            Exits early if end-of-command-upload flags read
        */
        for (int i = 0; i < sfr::rockblock::max_commands_count; i++) {
            uint8_t look_ahead1 = sfr::rockblock::serial.read(); // Peek
            uint8_t look_ahead2 = sfr::rockblock::serial.read(); // Peek
            if (look_ahead1 == constants::rockblock::end_of_command_upload_flag1 && look_ahead2 == constants::rockblock::end_of_command_upload_flag2) {
                break; // Exit command read loop
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

            // Already read first and second opcode indices; start at third index
            for (size_t o = 2; o < constants::rockblock::opcode_len; ++o) {
                new_raw_command.opcode[o] = sfr::rockblock::serial.read();
                if (new_raw_command.opcode[o] < 0x10)
                    Serial.print(0, HEX);
                Serial.print(new_raw_command.opcode[o], HEX);
            }
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
            } else if (new_raw_command.opcode[0] == 'F' && new_raw_command.opcode[1] == 'L') {
                Serial.println("SAT INFO: flush confirmed");
                sfr::rockblock::flush_status = false;
            } else {
                // Invalid Command Recieved
                // TODO: What Goes Here @Lauren
                Serial.println("SAT INFO: invalid command");
            }
        }

        sfr::rockblock::conseq_reads++;
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
    bool cycle_boundary = sfr::rockblock::conseq_reads >= (constants::rockblock::max_conseq_read - 1);
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
    if (sfr::rockblock::serial.read() == 'K') {
        sfr::rockblock::flush_status = true;
        Serial.println("SAT INFO: OK");
        transition_to(rockblock_mode_type::send_response);
    }
}

void RockblockControlTask::dispatch_end_transmission()
{
    sfr::rockblock::last_downlink = millis();
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
}

RockblockCommand *RockblockControlTask::commandFactory(RawRockblockCommand raw)
{
    // Create Specific Child Class of Rockblock command depending on the OP Code
    uint16_t op_code = raw.get_f_opcode();
    if (op_code <= constants::rockblock::opcodes::sfr_field_opcode_max && op_code >= constants::rockblock::opcodes::sfr_field_opcode_min) {
#ifdef VERBOSE_RB
        Serial.println("SFR Override Command");
#endif
        return new SFROverrideCommand(raw);
    } else if (op_code == constants::rockblock::opcodes::sfr_field_opcode_deploy) {
#ifdef VERBOSE_RB
        Serial.println("SFR Deploy Command");
#endif
        return new DeployCommand(raw);
    } else if (op_code == constants::rockblock::opcodes::sfr_field_opcode_arm) {
#ifdef VERBOSE_RB
        Serial.println("SFR Arm Command");
#endif
        return new ArmCommand(raw);
    } else if (op_code == constants::rockblock::opcodes::sfr_field_opcode_fire) {
#ifdef VERBOSE_RB
        Serial.println("SFR Fire Command");
#endif
        return new FireCommand(raw);
    } else {
#ifdef VERBOSE_RB
        Serial.print("Unknown Command with opcode: ");
        Serial.println(op_code, HEX);
#endif
        return new UnknownCommand(raw);
    }
}

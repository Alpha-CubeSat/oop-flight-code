#include "RockblockControlTask.hpp"

RockblockControlTask::RockblockControlTask(unsigned int offset)
    : TimedControlTask<void>(offset)
{
    sfr::rockblock::serial.begin(constants::rockblock::baud);
}

void RockblockControlTask::execute()
{
    //check_timeout();
    rockblock_mode_type mode = sfr::rockblock::mode;
#ifdef VERBOSE
    Serial.printf("Current rockblock mode: %d\n", mode);
#endif
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
    if (sfr::rockblock::rockblock_ready_status) {
        Serial.print("Rockblock Ready to Downlink\n");
    } else {
        Serial.print("Rockblock Not Ready to Downlink\n");
    }
#endif

    if ((sfr::rockblock::rockblock_ready_status || sfr::rockblock::waiting_message) && !sfr::rockblock::sleep_mode) {
        transition_to(rockblock_mode_type::send_at);
        Pins::setPinState(constants::rockblock::sleep_pin, HIGH);
    } else {
        Pins::setPinState(constants::rockblock::sleep_pin, LOW);
    }
}

void RockblockControlTask::dispatch_send_at()
{
    sfr::rockblock::conseq_reads = 0;
#ifdef VERBOSE
    Serial.println("SENT: ATr");
#endif
    sfr::rockblock::serial.print("AT\r");
    transition_to(rockblock_mode_type::await_at);
}

void RockblockControlTask::dispatch_await_at()
{
    if (sfr::rockblock::serial.read() == 'K') {
        Serial.println("SAT INFO: ok");
        transition_to(rockblock_mode_type::send_signal_strength);
        sfr::rockblock::start_time_check_signal = millis();
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
    if (sfr::rockblock::serial.read() == ':') {
        char signal = sfr::rockblock::serial.read();
        Serial.print("SAT INFO: signal level ");
        Serial.println(signal);
        if (signal == '3' || signal == '4' || signal == '5') {
            transition_to(rockblock_mode_type::send_flow_control);
        } else {
            transition_to(rockblock_mode_type::send_signal_strength);
        }
    }
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
    if (sfr::rockblock::serial.read() == 'K') {
        Serial.println("SAT INFO: ok");
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
    if (sfr::rockblock::serial.read() == 'Y') {
        Serial.println("SAT INFO: ready");
        transition_to(rockblock_mode_type::send_message);
    }
}

void RockblockControlTask::dispatch_send_message()
{
    uint16_t checksum = 0;
#ifdef VERBOSE
    switch (sfr::rockblock::downlink_report_type) {
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
#endif
#ifdef VERBOSE
    Serial.print("SENT: ");
#endif
    for (auto &data : sfr::rockblock::downlink_report) {
#ifdef VERBOSE
        if (data < 16) {
            Serial.print(0);
        }
        Serial.print(data, HEX);
#endif
        sfr::rockblock::serial.write(data);
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
            Serial.println("SAT INFO: report accepted");
            transition_to(rockblock_mode_type::send_response);
        } else {
            transition_to(rockblock_mode_type::send_message);
        }
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
#ifdef VERBOSE
    Serial.println("SENT: AT+CSQr");
#endif
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
        if (sfr::rockblock::downlink_report_type == report_type::camera_report) {
            sfr::camera::report_downlinked = true;
            sfr::rockblock::camera_report.clear();
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
#ifdef VERBOSE
    Serial.println("SENT: AT+SBDRBr");
#endif
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

        Serial.print("SAT CMD: ");
        for (size_t o = 0; o < constants::rockblock::opcode_len; ++o) {
            sfr::rockblock::opcode[o] = sfr::rockblock::serial.read();
            if (sfr::rockblock::opcode[o] < 0x10)
                Serial.print(0, HEX);
            Serial.print(sfr::rockblock::opcode[o], HEX);
        }
        for (size_t a1 = 0; a1 < constants::rockblock::arg1_len; ++a1) {
            sfr::rockblock::arg_1[a1] = sfr::rockblock::serial.read();
            if (sfr::rockblock::arg_1[a1] < 0x10)
                Serial.print(0, HEX);
            Serial.print(sfr::rockblock::arg_1[a1], HEX);
        }
        for (size_t a2 = 0; a2 < constants::rockblock::arg2_len; ++a2) {
            sfr::rockblock::arg_2[a2] = sfr::rockblock::serial.read();
            if (sfr::rockblock::arg_2[a2] < 0x10)
                Serial.print(0, HEX);
            Serial.print(sfr::rockblock::arg_2[a2], HEX);
        }
        Serial.println();

        if (valid_command()) {
            sfr::rockblock::f_opcode = (sfr::rockblock::opcode[1] << 8) | (sfr::rockblock::opcode[0]);
            sfr::rockblock::f_arg_1 = (sfr::rockblock::arg_1[3] << 24) | (sfr::rockblock::arg_1[2] << 16) | (sfr::rockblock::arg_1[1] << 8) | (sfr::rockblock::arg_1[0]);
            sfr::rockblock::f_arg_2 = (sfr::rockblock::arg_2[3] << 24) | (sfr::rockblock::arg_2[2] << 16) | (sfr::rockblock::arg_2[1] << 8) | (sfr::rockblock::arg_2[0]);

            sfr::rockblock::waiting_command = true;
        } else if (sfr::rockblock::opcode[0] == 'F' && sfr::rockblock::opcode[1] == 'L') {
            Serial.println("SAT INFO: flush confirmed");
            sfr::rockblock::flush_status = false;
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
#ifdef VERBOSE
    Serial.println("SENT: AT+SBDWT=FLUSH_MTr");
#endif
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
    if (sfr::rockblock::downlink_report_type == report_type::camera_report) {
        sfr::camera::report_downlinked = true;
        sfr::rockblock::camera_report.clear();
    }
    transition_to(rockblock_mode_type::standby);
}

void RockblockControlTask::transition_to(rockblock_mode_type new_mode)
{
    sfr::rockblock::mode = new_mode;
}

bool RockblockControlTask::valid_command()
{
    bool opcode = false;
    bool arg_1 = true;
    bool arg_2 = true;
    bool rockblock_downlink_period_opcode = true;
    bool request_image_fragment_opcode = true;
    bool burnwire_time_opcode = true;
    bool burnwire_timeout_opcode = true;

    // Check if opcode matches non-standard command (variable arg)
    for (size_t o = 0; o < constants::rockblock::opcode_len; o++) {
        if (sfr::rockblock::opcode[o] != constants::rockblock::request_image_fragment[o]) {
            request_image_fragment_opcode = false;
        }
        if (sfr::rockblock::opcode[o] != constants::rockblock::rockblock_downlink_period[o]) {
            rockblock_downlink_period_opcode = false;
        }
        if (sfr::rockblock::opcode[o] != constants::rockblock::burnwire_time[o]) {
            burnwire_time_opcode = false;
        }
        if (sfr::rockblock::opcode[o] != constants::rockblock::burnwire_timeout[o]) {
            burnwire_timeout_opcode = false;
        }
    }

    if (request_image_fragment_opcode) {
        for (size_t c = 0; c < 99; c++) {
            for (size_t a1 = 0; a1 < constants::rockblock::arg1_len; a1++) {
                if (sfr::rockblock::arg_1[a1] != sfr::rockblock::camera_commands[c][a1 + constants::rockblock::opcode_len]) {
                    arg_1 = false;
                }
            }
        }
    }

    bool non_std_cmd = (rockblock_downlink_period_opcode && arg_2) || (request_image_fragment_opcode && arg_1) || (burnwire_time_opcode) || (burnwire_timeout_opcode);
    if (non_std_cmd) {
        Serial.println("SAT CMD: command validated");
        return true;
    }

    // Loop over all standard commands
    for (size_t c = 0; c < constants::rockblock::num_commands; c++) {
        opcode = true;
        arg_1 = true;
        arg_2 = true;

        for (size_t o = 0; o < constants::rockblock::opcode_len; o++) {
            if (sfr::rockblock::opcode[o] != constants::rockblock::known_commands[c][o]) {
                opcode = false;
            }
        }
        for (size_t a1 = 0; a1 < constants::rockblock::arg1_len; a1++) {
            if (sfr::rockblock::arg_1[a1] != constants::rockblock::known_commands[c][a1 + constants::rockblock::opcode_len]) {
                arg_1 = false;
            }
        }
        for (size_t a2 = 0; a2 < constants::rockblock::arg2_len; a2++) {
            if (sfr::rockblock::arg_2[a2] != constants::rockblock::known_commands[c][a2 + constants::rockblock::opcode_len + constants::rockblock::arg1_len]) {
                arg_2 = false;
            }
        }

        if ((opcode && arg_1 && arg_2)) {
            Serial.println("SAT CMD: command validated");
            return true;
        }
    }

    // Command neither standard or non-standard
    Serial.println("SAT CMD: command invalid");
    return false;
}

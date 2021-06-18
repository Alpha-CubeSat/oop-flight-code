#include "RockblockControlTask.hpp"

RockblockControlTask::RockblockControlTask(unsigned int offset): TimedControlTask<void>(offset){
    sfr::rockblock::serial.begin(constants::rockblock::baud);
}

void RockblockControlTask::execute(){
    if(sfr::rockblock::num_iter > constants::rockblock::max_iter){
        transition_to(rockblock_mode_type::standby);
    }
    rockblock_mode_type mode = sfr::rockblock::mode;
    sfr::rockblock::num_iter++;
    switch(mode){
        case rockblock_mode_type::standby:
            Serial.println("standby");
            dispatch_standby();
            break;
        case rockblock_mode_type::send_at:
            Serial.println("send_at");
            dispatch_send_at();
            break;
        case rockblock_mode_type::await_at:
            Serial.println("await_at");
            dispatch_await_at();
            break;
        case rockblock_mode_type::send_signal_strength:
            Serial.println("send_signal_strength");
            dispatch_send_signal_strength();
            break;
        case rockblock_mode_type::await_signal_strength:
            Serial.println("await_signal_strength");
            dispatch_await_signal_strength();
            break;
        case rockblock_mode_type::send_flow_control:
            Serial.println("send_flow_control");
            dispatch_send_flow_control();
            break;
        case rockblock_mode_type::await_flow_control:
            Serial.println("await_flow_control");
            dispatch_await_flow_control();
            break;
        case rockblock_mode_type::send_message_length:
            Serial.println("send_message_length");
            dispatch_send_message_length();
            break;
        case rockblock_mode_type::await_message_length:
            Serial.println("await_message_length");
            dispatch_await_message_length();
            break;
        case rockblock_mode_type::send_message:
            Serial.println("send_message");
            dispatch_send_message(); 
            break;
        case rockblock_mode_type::await_message:
            Serial.println("await_message");
            dispatch_await_message();
            break; 
        case rockblock_mode_type::send_response:
            Serial.println("send_response");
            dispatch_send_response(); 
            break;
        case rockblock_mode_type::create_buffer:
            Serial.println("create_buffer");
            dispatch_create_buffer(); 
            break;
        case rockblock_mode_type::process_mo_status:
            Serial.println("process_mo_status");
            dispatch_process_mo_status(); 
            break;
        case rockblock_mode_type::process_mt_status:
            Serial.println("process_mt_status");
            dispatch_process_mt_status();
            break;
        case rockblock_mode_type::read_message:
            Serial.println("read_message");
            dispatch_read_message();
            break;
        case rockblock_mode_type::process_command:
            Serial.println("process_command");
            dispatch_process_command();
            break;
        case rockblock_mode_type::queue_check:
            Serial.println("queue_check");
            dispatch_queue_check();
            break;
        case rockblock_mode_type::send_flush:
            Serial.println("send_flush");
            dispatch_send_flush();
            break;
        case rockblock_mode_type::await_flush:
            Serial.println("await_flush");
            dispatch_await_flush();
            break;
        case rockblock_mode_type::end_transmission:
            Serial.println("end_transmission");
            dispatch_end_transmission();
            break;
    }
}

bool RockblockControlTask::check_ready(){
    if(millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period){
        return true;
    } else{
        return false;
    }
}

void RockblockControlTask::dispatch_standby(){
    if(sfr::rockblock::waiting_message || check_ready()){
        transition_to(rockblock_mode_type::send_at);
    }
    transition_to(rockblock_mode_type::send_at);
}

void RockblockControlTask::dispatch_send_at(){
    sfr::rockblock::serial.print("AT\r");
    transition_to(rockblock_mode_type::await_at);
}

void RockblockControlTask::dispatch_await_at(){
    if(sfr::rockblock::serial.read()=='K'){
        transition_to(rockblock_mode_type::send_signal_strength);
    }
}

void RockblockControlTask::dispatch_send_signal_strength(){
    sfr::rockblock::serial.print("AT+CSQ\r");
    transition_to(rockblock_mode_type::await_signal_strength);
}

void RockblockControlTask::dispatch_await_signal_strength(){
    if(sfr::rockblock::serial.read()==':'){
        char signal = sfr::rockblock::serial.read();
        Serial.print("SIGNAL: ");
        Serial.println(signal);
        if(signal =='3' || signal =='4' || signal =='5'){
            transition_to(rockblock_mode_type::send_flow_control);
        } else{
            transition_to(rockblock_mode_type::send_signal_strength);
        }
    }
}

void RockblockControlTask::dispatch_send_flow_control(){
    sfr::rockblock::serial.print("AT&K0\r");
    transition_to(rockblock_mode_type::await_flow_control);
}

void RockblockControlTask::dispatch_await_flow_control(){
    if(sfr::rockblock::serial.read()=='K'){
        transition_to(rockblock_mode_type::send_message_length);
    } 
}

void RockblockControlTask::dispatch_send_message_length(){
    sfr::rockblock::serial.print("AT+SBDWB=70\r");
    transition_to(rockblock_mode_type::await_message_length);
}

void RockblockControlTask::dispatch_await_message_length(){
    if(sfr::rockblock::serial.read()=='Y'){
        transition_to(rockblock_mode_type::send_message);
    } 
}

void RockblockControlTask::dispatch_send_message(){
    uint16_t checksum = 0;
    // Serial.print("DATA: ");
    for (size_t i=0; i < constants::rockblock::packet_size; ++i){
        // Serial.print(sfr::rockblock::report[i]);
        sfr::rockblock::serial.write(sfr::rockblock::report[i]);
        checksum += (uint16_t) sfr::rockblock::report[i];
    }
    // Serial.println();
    // Serial.print("CHKSM: ");
    // Serial.print(checksum >> 8);
    // Serial.print(checksum & 0xFF);
    // Serial.println();
    sfr::rockblock::serial.write(checksum >> 8);
    sfr::rockblock::serial.write(checksum & 0xFF);
    sfr::rockblock::serial.write('\r');
    transition_to(rockblock_mode_type::await_message);
}

void RockblockControlTask::dispatch_await_message(){
    char c = sfr::rockblock::serial.read();
    if(c == '0' || c == '1' || c == '2' || c == '3'){
        if(c == '0'){
            transition_to(rockblock_mode_type::send_response);
        }
        else{
            transition_to(rockblock_mode_type::send_message);
        }
    }
}

void RockblockControlTask::dispatch_send_response(){
    sfr::rockblock::serial.print("AT+SBDIX\r");
    transition_to(rockblock_mode_type::create_buffer);
}

void RockblockControlTask::dispatch_create_buffer(){
    if(sfr::rockblock::serial.read() == ':'){
        // clear buffer to nulls
        memset(sfr::rockblock::buffer, '\0', constants::rockblock::buffer_size);
        // clear commas to -1
        memset(sfr::rockblock::commas, -1, constants::rockblock::num_commas);
        // int relevant_chars = sfr::rockblock::serial.available()-8;
        int buffer_iter = 0;
        int comma_iter = 0;
        for(size_t i = 0; i < constants::rockblock::buffer_size; i++) {
            char c = sfr::rockblock::serial.read();
            if(c == '\r') {
                break;
            }
            if(c != ' '){
                sfr::rockblock::buffer[buffer_iter] = c;
                Serial.print( sfr::rockblock::buffer[buffer_iter]);
                if(c == ','){
                    sfr::rockblock::commas[comma_iter] = buffer_iter;
                    comma_iter++;
                }
                buffer_iter++;
            }
        }
        Serial.println();
        if(comma_iter != 5){
            transition_to(rockblock_mode_type::send_response);
        }
        else{
            transition_to(rockblock_mode_type::process_mo_status);
        }
                    
    }
}

void RockblockControlTask::dispatch_process_mo_status(){
    if(sfr::rockblock::commas[0] > 1){
        Serial.println("there is another character");
        transition_to(rockblock_mode_type::send_response);
    }
    else if(sfr::rockblock::buffer[0] != '0' && sfr::rockblock::buffer[0] != '1' && sfr::rockblock::buffer[0] != '2'){
        Serial.println("mo status is greater than 2");
        transition_to(rockblock_mode_type::send_response);
    } else{
        transition_to(rockblock_mode_type::process_mt_status);
    }             
}

void RockblockControlTask::dispatch_process_mt_status(){
    switch(sfr::rockblock::buffer[sfr::rockblock::commas[1]+1]){
        case '2':
            Serial.println("error during check");
            transition_to(rockblock_mode_type::send_response);
            break;
        case '1':
            Serial.println("message retrieved");
            transition_to(rockblock_mode_type::read_message);
            break;
        case '0':
            Serial.println("there were no messages to retrieve");
            transition_to(rockblock_mode_type::end_transmission);
            break;
    }              
}

void RockblockControlTask::dispatch_read_message(){
    sfr::rockblock::serial.print("AT+SBDRB\r");
    transition_to(rockblock_mode_type::process_command);
}

void RockblockControlTask::dispatch_process_command(){
    if(sfr::rockblock::serial.read() == 'B'){
        sfr::rockblock::serial.read();
        sfr::rockblock::serial.read();
        sfr::rockblock::serial.read();
        sfr::rockblock::serial.read();
        sfr::rockblock::serial.read();
        sfr::rockblock::serial.read();


        Serial.print("COMMAND: ");
        for (size_t o=0; o<sizeof(sfr::rockblock::opcode); ++o){
            sfr::rockblock::opcode[o] = sfr::rockblock::serial.read();
            if( sfr::rockblock::opcode[o] < 0x10 ) Serial.print(0, HEX);
            Serial.print(sfr::rockblock::opcode[o], HEX);
        }
        for (size_t a1=0; a1<sizeof(sfr::rockblock::arg_1); ++a1){
            sfr::rockblock::arg_1[a1] = sfr::rockblock::serial.read();
            if( sfr::rockblock::arg_1[a1] < 0x10 ) Serial.print(0, HEX);
            Serial.print(sfr::rockblock::arg_1[a1], HEX);
        }
        for (size_t a2=0; a2<sizeof(sfr::rockblock::arg_2); ++a2){
            sfr::rockblock::arg_2[a2] = sfr::rockblock::serial.read();
            if( sfr::rockblock::arg_2[a2] < 0x10 ) Serial.print(0, HEX);
            Serial.print(sfr::rockblock::arg_2[a2], HEX);
        }
        Serial.println();

        if(valid_command()){
            uint16_t c_opcode = sfr::rockblock::opcode[0] | (sfr::rockblock::opcode[1] << 8);
            uint32_t c_arg_1 = sfr::rockblock::arg_1[0] | (sfr::rockblock::arg_1[1] << 8) | (sfr::rockblock::arg_1[2] << 16) | (sfr::rockblock::arg_1[3] << 24);
            uint32_t c_arg_2 = sfr::rockblock::arg_2[0] | (sfr::rockblock::arg_2[1] << 8) | (sfr::rockblock::arg_2[2] << 16) | (sfr::rockblock::arg_2[3] << 24);

            std::stringstream ss_opcode;
            ss_opcode << c_opcode;
            int f_opcode;
            ss_opcode >> f_opcode;

            std::stringstream ss_arg_1;
            ss_arg_1 << c_arg_1;
            int f_arg_1;
            ss_arg_1 >> f_arg_1;

            std::stringstream ss_arg_2;
            ss_arg_2 << c_arg_2;
            int f_arg_2;
            ss_arg_2 >> f_arg_2;

            sfr::rockblock::f_opcode = f_opcode;
            sfr::rockblock::f_arg_1 = f_arg_1;
            sfr::rockblock::f_arg_2 = f_arg_2;
            
            sfr::rockblock::waiting_command = true;
        }
        transition_to(rockblock_mode_type::queue_check);
    }
}

void RockblockControlTask::dispatch_queue_check() {
    size_t idx = sfr::rockblock::commas[4] + 1;
    char* ptr = sfr::rockblock::buffer + idx;
    int len = strtol(ptr, nullptr, 10);
    Serial.print("waiting messages: ");
    Serial.println(len);
    if( len >= constants::rockblock::max_queue ) {
        transition_to(rockblock_mode_type::send_flush);
    } else if(len > 0) {
        transition_to(rockblock_mode_type::send_response);
    } else {
        transition_to(rockblock_mode_type::end_transmission);
    }
    
}

void RockblockControlTask::dispatch_send_flush(){
    sfr::rockblock::serial.print("AT+SBDWT=FLUSH_MT\r");
    transition_to(rockblock_mode_type::await_flush);
}

void RockblockControlTask::dispatch_await_flush() {
    if(sfr::rockblock::serial.read()=='K'){
        transition_to(rockblock_mode_type::send_response);
    }
}

void RockblockControlTask::dispatch_end_transmission(){
    sfr::rockblock::last_downlink = millis();
    transition_to(rockblock_mode_type::standby);
}

void RockblockControlTask::transition_to(rockblock_mode_type new_mode){
    sfr::rockblock::mode = new_mode;
    sfr::rockblock::num_iter = 0;
}

bool RockblockControlTask::valid_command(){
    bool opcode = false;
    bool arg_1 = false;
    bool arg_2 = false;

    for( size_t c = 0; c < constants::rockblock::num_commands; c++ ) {
        /*opcode = true;
        arg_1 = true;
        arg_2 = true;

        for( size_t o = 0; o < constants::rockblock::opcode_len; o++ ) {
            if(sfr::rockblock::opcode[o] != constants::rockblock::known_commands[c][o]) {
                opcode = false;
            }
        }
        for( size_t a1 = 0; a1 < constants::rockblock::arg1_len; a1++ ) {
            if(sfr::rockblock::arg_1[a1] != constants::rockblock::known_commands[c][a1 + constants::rockblock::opcode_len]) {
                arg_1 = false;
            }  
        }
        for( size_t a2 = 0; a2 < constants::rockblock::arg2_len; a2++ ) {
            if(sfr::rockblock::arg_2[a2] != constants::rockblock::known_commands[c][a2 + constants::rockblock::opcode_len + constants::rockblock::arg1_len]) {
                arg_2 = false;
            }     
        }

        if( opcode && arg_1 && arg_2 ) {
            Serial.println("command validated");
            return true;
        }*/
    }

    Serial.println("command invalid");
    return false;
}
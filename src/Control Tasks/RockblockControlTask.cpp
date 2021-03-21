#include "RockblockControlTask.hpp"

#ifndef GIT
RockblockControlTask::RockblockControlTask(){
    Serial4.begin(constants::rockblock::baud);
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
        case rockblock_mode_type::process_opcode:
            Serial.println("process_opcode");
            dispatch_process_opcode();
            break;
        case rockblock_mode_type::process_argument:
            Serial.println("process_argument");
            dispatch_process_argument();
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
    /*if(sfr::rockblock::waiting_message || check_ready()){
        transition_to(rockblock_mode_type::send_at);
    }*/
    transition_to(rockblock_mode_type::send_at);
}

void RockblockControlTask::dispatch_send_at(){
    Serial4.print("AT\r");
    transition_to(rockblock_mode_type::await_at);
}

void RockblockControlTask::dispatch_await_at(){
    if(Serial4.read()==constants::ascii::K){
        transition_to(rockblock_mode_type::send_flow_control);
    } 
}

void RockblockControlTask::dispatch_send_flow_control(){
    Serial4.print("AT&K0\r");
    transition_to(rockblock_mode_type::await_flow_control);
}

void RockblockControlTask::dispatch_await_flow_control(){
    if(Serial4.read()==constants::ascii::K){
        transition_to(rockblock_mode_type::send_message_length);
    } 
}

void RockblockControlTask::dispatch_send_message_length(){
    Serial4.print("AT+SBDWB=70\r");
    transition_to(rockblock_mode_type::await_message_length);
}

void RockblockControlTask::dispatch_await_message_length(){
    if(Serial4.read()==constants::ascii::Y){
        transition_to(rockblock_mode_type::send_message);
    } 
}

void RockblockControlTask::dispatch_send_message(){
    uint16_t checksum = 0;
    for (size_t i=0; i<sizeof(sfr::rockblock::report); ++i){
        Serial4.write(sfr::rockblock::report[i]);
        checksum += (uint16_t)sfr::rockblock::report[i];
    }
    Serial4.write(checksum >> 8);
    Serial4.write(checksum & 0xFF);
    transition_to(rockblock_mode_type::await_message);
}

void RockblockControlTask::dispatch_await_message(){
    uint8_t c = Serial4.read();
    if(c == constants::ascii::zero || c == constants::ascii::one || c == constants::ascii::two || c == constants::ascii::three){
        if(c == constants::ascii::zero){
            transition_to(rockblock_mode_type::send_response);
        }
        else{
            transition_to(rockblock_mode_type::send_message);
        }
    }
}

void RockblockControlTask::dispatch_send_response(){
    Serial4.print("AT+SBDIX\r");
    transition_to(rockblock_mode_type::create_buffer);
}

void RockblockControlTask::dispatch_create_buffer(){
    if(Serial4.read() == 58){
        int relevant_chars = Serial4.available()-8;
        int buffer_iter = 0;
        int comma_iter = 0;
        for (int i=0; i<relevant_chars; ++i){
            uint8_t c = Serial4.read();
            if(c != 32){
                sfr::rockblock::buffer[buffer_iter] = c;
                Serial.println( sfr::rockblock::buffer[buffer_iter]);
                if(c == 44){
                    sfr::rockblock::commas[comma_iter] = buffer_iter;
                    comma_iter++;
                }
                buffer_iter++;
            }
        }
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
            Serial.println("there are messages waiting");
            transition_to(rockblock_mode_type::read_message);
            break;
        case '0':
            Serial.println("there were no messages to retrieve");
            transition_to(rockblock_mode_type::standby);
            break;
    }              
}

void RockblockControlTask::dispatch_read_message(){
    Serial4.print("AT+SBDRB\r");
    transition_to(rockblock_mode_type::process_opcode);
}

void RockblockControlTask::dispatch_process_opcode(){
    if(Serial4.read() == constants::ascii::exclamation){
        sfr::rockblock::opcode[0] = Serial4.read();
        sfr::rockblock::opcode[1] = Serial4.read();
        Serial.println(sfr::rockblock::opcode[0]);
        Serial.println(sfr::rockblock::opcode[1]);
        transition_to(rockblock_mode_type::process_argument); 
    }    
}

void RockblockControlTask::dispatch_process_argument(){
    if(Serial4.read() == constants::ascii::exclamation){
        sfr::rockblock::argument[0] = Serial4.read();
        sfr::rockblock::argument[1] = Serial4.read();
        Serial.println(sfr::rockblock::argument[0]);
        Serial.println(sfr::rockblock::argument[1]);
        transition_to(rockblock_mode_type::standby); 
    } 
    
    
}

void RockblockControlTask::transition_to(rockblock_mode_type new_mode){
    sfr::rockblock::mode = new_mode;
    sfr::rockblock::num_iter = 0;
}

#else
RockblockControlTask::RockblockControlTask(){}
void RockblockControlTask::execute(){}
bool RockblockControlTask::checkReady(){}
#endif
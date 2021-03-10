#include "RockblockControlTask.hpp"

#ifndef GIT
RockblockControlTask::RockblockControlTask(){
    Serial4.begin(constants::rockblock::baud);
}

void RockblockControlTask::execute(){
    rockblock_mode_type mode = sfr::rockblock::mode;
    switch(mode){
        case rockblock_mode_type::standby:
            {
                Serial.println("standby");
                if(sfr::rockblock::waiting_message || checkReady()){
                    sfr::rockblock::mode = rockblock_mode_type::send_at;
                }
            }
        case rockblock_mode_type::send_at:
            {
                Serial.println("send_at");
                Serial4.print("AT\r");
                sfr::rockblock::mode = rockblock_mode_type::await_at;
                break;
            }
        case rockblock_mode_type::await_at:
            {
                Serial.println("await_at");
                if(Serial4.read() == 79 && Serial4.read()==75){
                    sfr::rockblock::mode = rockblock_mode_type::send_flow_control;
                } 
                break;
            }
        case rockblock_mode_type::send_flow_control:
            {
                Serial.println("send_flow_control");
                Serial4.print("AT&K0\r");
                sfr::rockblock::mode = rockblock_mode_type::await_flow_control;
                break;
            }
        case rockblock_mode_type::await_flow_control:
            {
                Serial.println("await_flow_control");
                if(Serial4.read() == 79 && Serial4.read()==75){
                    sfr::rockblock::mode = rockblock_mode_type::send_message_length;
                } 
                break;
            }
        case rockblock_mode_type::send_message_length:
            {
                Serial.println("send_message_length");
                Serial4.print("AT+SBDWB=70\r");
                sfr::rockblock::mode = rockblock_mode_type::await_message_length;
                break;
            }
        case rockblock_mode_type::await_message_length:
            {
                Serial.println("await_message_length");
                if(Serial4.read() == 82 && Serial4.read()==69 && Serial4.read()==65 && Serial4.read()==68 && Serial4.read()==89){
                    sfr::rockblock::mode = rockblock_mode_type::send_message;
                } 
                break;
            }
        case rockblock_mode_type::send_message:
            {
                Serial.println("send_message");
                uint16_t checksum = 0;
                for (size_t i=0; i<sizeof(sfr::rockblock::report); ++i){
                    Serial4.write(sfr::rockblock::report[i]);
                    checksum += (uint16_t)sfr::rockblock::report[i];
                }
                Serial4.write(checksum >> 8);
                Serial4.write(checksum & 0xFF);
                sfr::rockblock::mode = rockblock_mode_type::await_message;
                break;
            }
        case rockblock_mode_type::await_message:
            {
                Serial.println("await_message");
                uint8_t c = Serial4.read();
                if(c == 48 || c == 49 || c == 50 || c == 51){
                    if(c == 48){
                        sfr::rockblock::mode = rockblock_mode_type::send_response;
                    }
                    else{
                        sfr::rockblock::mode = rockblock_mode_type::send_message;
                    }
                }
                break;
            }
        case rockblock_mode_type::send_response:
            {
                Serial.println("send response");
                Serial4.print("AT+SBDIX\r");
                sfr::rockblock::mode = rockblock_mode_type::create_buffer;
                break;
            }
        case rockblock_mode_type::create_buffer:
            {
                Serial.println("create buffer");
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
                        sfr::rockblock::mode = rockblock_mode_type::send_response;
                    }
                    else{
                        sfr::rockblock::mode = rockblock_mode_type::process_mo_status;
                    }
                    
                }
                break;
            }
        case rockblock_mode_type::process_mo_status:
            {
                Serial.println("process mo status");
                if(sfr::rockblock::commas[0] > 1){
                    Serial.println("there is another character");
                    sfr::rockblock::mode = rockblock_mode_type::send_response;
                    break;
                }
                if(sfr::rockblock::buffer[0] != '0' && sfr::rockblock::buffer[0] != '1' && sfr::rockblock::buffer[0] != '2'){
                    Serial.println("mo status is greater than 2");
                    sfr::rockblock::mode = rockblock_mode_type::send_response;
                    break;
                }
                sfr::rockblock::mode = rockblock_mode_type::process_mt_status;
                break;
            }
        case rockblock_mode_type::process_mt_status:
            {
                Serial.println("process mt status");
                switch(sfr::rockblock::buffer[sfr::rockblock::commas[1]+1]){
                    case '2':
                        {
                            Serial.println("error during check");
                            sfr::rockblock::mode = rockblock_mode_type::send_response;
                            break;
                        }
                    case '1':
                        {
                            Serial.println("there are messages waiting");
                            sfr::rockblock::mode = rockblock_mode_type::read_message;
                            break;
                        }
                    case '0':
                        {
                            Serial.println("there were no messages to retrieve");
                            delay(5000);
                            sfr::rockblock::mode = rockblock_mode_type::standby;
                            break;
                        }
                }
                break;
            }
        case rockblock_mode_type::read_message:
        {
            Serial.println("read message");
            Serial4.print("AT+SBDRB\r");
            sfr::rockblock::mode = rockblock_mode_type::process_opcode;
            break;
        }
        case rockblock_mode_type::process_opcode:
        {
            Serial.println("process opcode");
            if(Serial4.read() == 33){
                for (int i=0; i<Serial4.available(); ++i){
                    if(Serial4.read() != 33){
                        sfr::rockblock::opcode[i] = Serial4.read();
                    }else{
                        sfr::rockblock::mode = rockblock_mode_type::process_argument;
                    }
                }  
            }
        }
        case rockblock_mode_type::process_argument:
        {
            Serial.println("process argument");
            for (int i=0; i<Serial4.available(); ++i){
                if(Serial4.read() != 33){
                    sfr::rockblock::opcode[i] = Serial4.read();
                }else{
                    sfr::rockblock::mode = rockblock_mode_type::standby;
                }
            }  
        }
    }
}

bool RockblockControlTask::checkReady(){
    if(millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period){
        return true;
    } else{
        return false;
    }
}
#else
RockblockControlTask::RockblockControlTask(){}
void RockblockControlTask::execute(){}
bool RockblockControlTask::checkReady(){}
#endif
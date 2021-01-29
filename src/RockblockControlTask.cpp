#include "RockblockControlTask.hpp"

RockblockControlTask::RockblockControlTask():
    rockblock(Serial4){
    Serial4.begin(19200);
}

void RockblockControlTask::execute(){
    rockblock_mode_type mode = sfr::rockblock::mode;

    //if(waiting_messages > 0 || checkReady()){
    if(true){
        uint8_t report[70] = {0};

        switch(mode){
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
                    while(Serial4.available()){
                        if(Serial4.read() == 79 && Serial4.read()==75){
                            sfr::rockblock::mode = rockblock_mode_type::send_flow_control;
                        } 
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
                    while(Serial4.available()){
                        if(Serial4.read() == 79 && Serial4.read()==75){
                            sfr::rockblock::mode = rockblock_mode_type::send_message_length;
                        } 
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
                    while(Serial4.available()){
                        if(Serial4.read() == 82 && Serial4.read()==69 && Serial4.read()==65 && Serial4.read()==68 && Serial4.read()==89){
                            sfr::rockblock::mode = rockblock_mode_type::send_message;
                        } 
                    }
                    break;
                }
            case rockblock_mode_type::send_message:
                {
                    Serial.println("send_message");
                    uint16_t checksum = 0;
                    for (size_t i=0; i<sizeof(report); ++i){
                        Serial4.write(report[i]);
                        checksum += (uint16_t)report[i];
                    }
                    Serial4.write(checksum >> 8);
                    Serial4.write(checksum & 0xFF);
                    sfr::rockblock::mode = rockblock_mode_type::await_message;
                    break;
                }
            case rockblock_mode_type::await_message:
                {
                    Serial.println("await_message");
                    while(Serial4.available()){
                        uint8_t c = Serial4.read();
                        if(c == 48 || c == 49 || c == 50 || c == 51){
                            if(c == 48){
                                sfr::rockblock::mode = rockblock_mode_type::send_response;
                            }
                            else{
                                sfr::rockblock::mode = rockblock_mode_type::send_message;
                            }
                        }
                    }
                    break;
                }
            case rockblock_mode_type::send_response:
                {
                    Serial.println("send response");
                    Serial4.print("AT+SBDIX\r");
                    sfr::rockblock::mode = rockblock_mode_type::await_response;
                    break;
                }
            case rockblock_mode_type::await_response:
                {
                    Serial.println("await response");
                    while(Serial4.available()){
                        if(Serial4.read() == 58){
                            uint8_t c = Serial4.read();
                            if(c == 44){
                                
                            }


                        }



                    }
                    /*if(Serial4.available() && Serial4.read() != 65){
                        while(Serial4.available()){
                            Serial.println(Serial4.read());
                        }




                        uint8_t buffer[Serial4.available()] = {0};
                        for (size_t i=0; i<sizeof(buffer); ++i){
                            buffer[i] = Serial4.read();
                            Serial.println(buffer[i], 19200);
                        }
                    }
                    else{
                        while(Serial4.available()){
                            Serial4.read();
                        }
                    }*/
                    
                    break;
                }
        }
    }
    
}

void RockblockControlTask::handleCommand(int opcode, int argument){
    Serial.print("the opcode is: ");
    Serial.println(opcode);
    Serial.print("the argument is: ");
    Serial.println(argument);
}

bool RockblockControlTask::checkReady(){
    if(millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period){
        return true;
    } else{
        return false;
    }
}
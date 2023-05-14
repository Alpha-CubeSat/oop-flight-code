#include <SerialTalk.hpp>
#include <SFRField.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

SerialTalk::SerialTalk(){
  logged = new std::deque<int>();
  log_rate = 10;
  log_time = 0;
}

void SerialTalk::execute(){
  if(log_time == log_rate){
    DynamicJsonDocument j(200);
    j["time"] = micros();
    int fields = 0;
    for(int i = 0; i < logged->size(); i++){
      fields++;
      int hex_num = logged->at(i);
      j[hex_num] = SFRInterface::opcode_lookup[hex_num]->getFieldValue();
    }
    if(fields>0){
      String json_str;
      serializeJson(j, json_str);
      Serial.print("LOG: ");
      Serial.println(json_str);
    }
    
    
  }

  
  if (Serial.available()) {
    char line[128];
    int len = Serial.readBytesUntil('\n', line, sizeof(line));
    line[len] = '\0'; // Null-terminate the string

    std::string input(line);
    if (input.substr(0, 13) == "debug_console") {
      std::istringstream ss(input);
      std::string token;
      std::vector<std::string> tokens;
      while (std::getline(ss, token, '.')) {
          tokens.push_back(token);
      }
      std::string response = "EMPTY";
      if(tokens[1] == "read"){
        int hex_num = strtol(tokens[2].c_str(), NULL, 16);
        int val = SFRInterface::opcode_lookup[hex_num]->getFieldValue();
        std::string t = std::string(String(val).c_str());
        response = t;
      }else if (tokens[1] == "log"){
        int hex_num = strtol(tokens[2].c_str(), NULL, 16);
        logged->push_front(hex_num);
      }
      
      Serial.print("Response: ");
      Serial.println(response.c_str());
    }
  }
}
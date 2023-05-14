#ifndef SERIAL_TALK_HPP
#define SERIAL_TALK_HPP

#include "Arduino.h"
#include <ArduinoJson.h>
#include <deque>


class SerialTalk
{
public:
    SerialTalk();
    void execute();
private:
  std::deque<int>* logged;
  int log_rate;
  int log_time;
};

#endif
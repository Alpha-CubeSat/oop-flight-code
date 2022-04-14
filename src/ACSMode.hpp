#ifndef ACS_MODE_HPP_
#define ACS_MODE_HPP_

#include "sfr.hpp"

class ACSMode
{
public:
    int id;
    float max_time;
    void set_max_time(float new_max_time){
        max_time = new_max_time;
    }
};

class Simple : public ACSMode
{
public:
    float max_time = 0;
    int id = 2;
    
};

class Point : public ACSMode
{
public:
    float max_time = 0;
    int id = 1;
};

class Off : public ACSMode
{
public:
    float max_time = 0;
    int id = 0;
};

#endif
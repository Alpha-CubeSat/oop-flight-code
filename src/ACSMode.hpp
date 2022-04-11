#ifndef ACS_MODE_HPP_
#define ACS_MODE_HPP_

#include "sfr.hpp"

class ACSMode
{
public:
    virtual float max_time();
    virtual int id();
};

class Simple : public ACSMode
{
public:
    float max_time(){
        //return sfr::simple::max_time;
        return 0;
    }
    int id(){
        return 2;
    }
    
};

class Point : public ACSMode
{
public:
    float max_time(){
        //return sfr::point::max_time;
        return 0;
    }
    int id(){
        return 1;
    }
};

class Off : public ACSMode
{
public:
    float max_time(){
        //return sfr::point::max_time;
        return 0;
    }
    int id(){
        return 0;
    }
};

#endif